#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

static volatile sig_atomic_t server_down = 0;

void signal_handler(int sig)
{
    if (sig == SIGINT || sig == SIGTERM)
        server_down = 1;
}

struct token_list{
    char **argv;
    int size;
    int used;
};

void add_to_list(struct token_list *tlist, char *str)
{
    if (tlist->size == tlist->used){
        tlist->size += 10;
        tlist->argv = realloc(tlist->argv, tlist->size*sizeof(char *));
    }
    tlist->argv[tlist->used] = str;
    tlist->used++;
}

void delete_token_list(struct token_list *tlist)
{
    int i;
    for (i = 0; i < tlist->used; i++)
        free(tlist->argv[i]);
    free(tlist->argv);
    free(tlist);
}

static char *copy_str(char *str, int size)
{
    char *copy = malloc(size + 1);
    int i;
    for (i = 0; i < size; i++){
        copy[i] = str[i];
    }
    copy[size] = '\0';
    return copy;
}

struct token_list *read_tokens(const char *str)
{
    int i, used = 0, flag = 0;
    char buf[256];
    struct token_list *tlist = malloc(sizeof(struct token_list));
    memset(tlist, 0, sizeof(struct token_list));
    for(i = 0; str[i]; i++){
        if (str[i] == '"'){
            flag = !flag;
            continue;
        }
        if ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n') && !flag){
            if (used > 0){
                add_to_list(tlist, copy_str(buf, used));
                used = 0;
            }
            continue;
        }
        if ((unsigned)used >= sizeof(buf)){
            delete_token_list(tlist);
            return NULL;
        }
        buf[used++] = str[i];
    }

    if (used > 0){
        add_to_list(tlist, copy_str(buf, used));
    }
    add_to_list(tlist, NULL);
    tlist->used--;
    return tlist;
}

enum client_state { st_wait_name, st_finished, st_online, st_admin};

struct private_names {
    char *name;
    struct private_names *next;
};

struct client {
    int sockfd;
    char buf[1024];
    int buf_used;
    char *name;
    enum client_state state;
    struct private_names *p_names;
    int admin_rights;
    int private_msgs_counter;
};

struct client_list {
    struct client *sess;
    struct client_list *next;
};

struct chat_server {
    struct client_list *sessions;
    int ls;
    char *admin_password;
    char *max_private_name;
    int max_private;
};

void add_client(struct client_list **list, struct client *cl)
{
    struct client_list *head = malloc(sizeof(struct client_list));
    head->sess = cl;
    head->next = *list;
    *list = head;
}

void delete_private_names(struct client *cl);

void delete_client(struct client *cl)
{
    shutdown(cl->sockfd, 2);
    close(cl->sockfd);
    delete_private_names(cl);
    free(cl->name);
    free(cl);
}

void delete_clients(struct client_list *list)
{
    struct client_list *tmp;
    while (list){
        tmp = list;
        list = list->next;
        delete_client(tmp->sess);
        free(tmp);
    }
}

void add_to_private_names(struct client *cl, const char *name)
{
    struct private_names *tmp;

    for (tmp = cl->p_names; tmp; tmp = tmp->next){
        if (!strcmp(tmp->name, name))
            return;
    }

    tmp = malloc(sizeof(struct private_names));
    tmp->next = cl->p_names;
    tmp->name = strdup(name);
    cl->p_names = tmp;
}

void delete_private_names(struct client *cl)
{
    struct private_names *current;
    struct private_names *next;
    
    if (cl == NULL)
        return;
    
    current = cl->p_names;
    while (current != NULL) {
        next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    
    cl->p_names = NULL;
}

void finish_sessions(struct client_list **list)
{
    while (*list){
        if ((*list)->sess->state == st_finished){
            struct client_list *tmp = *list;
            *list = (*list)->next;
            delete_client(tmp->sess);
            free(tmp);
        }
        else {
            list = &(*list)->next;
        }
    }
}

void send_string(struct client *cl, const char *str)
{
    write(cl->sockfd, str, strlen(str));
}

void broadcast_message(struct client_list *list, const char *mesg)
{
    struct client_list *tmp;
    for (tmp = list; tmp; tmp = tmp->next)
        send_string(tmp->sess, mesg);
}

void accept_client(struct chat_server *cs)
{
    struct sockaddr_in addr;
    struct client *cl;
    socklen_t addrlen = sizeof(addr);
    int sock = accept(cs->ls, (struct sockaddr *)&addr, &addrlen);
    if (sock < 0){
        perror("accept");
        return;
    }
    cl = malloc(sizeof(struct client));
    memset(cl, 0, sizeof(struct client));
    cl->name = NULL;
    cl->sockfd = sock;
    cl->state = st_wait_name;
    cl->p_names = NULL;
    cl->private_msgs_counter = 0;
    cl->admin_rights = 0;
    add_client(&cs->sessions, cl);
    send_string(cl, "Hello, enter your name: ");
    fprintf(stderr, "connection from %s:%u\n",
        inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

struct client *find_client(struct chat_server *cs, const char *name)
{
    struct client_list *tmp;
    for (tmp = cs->sessions; tmp; tmp = tmp->next){
        if (!tmp->sess->name)
            continue;
        if (!strcmp(tmp->sess->name, name))
            return tmp->sess;
    }
    return NULL;
}

void name_handling(struct chat_server *cs, struct client *cl, const char *str)
{
        char buf[128];
        int len = strlen(str);
        struct client *is_name_taken = find_client(cs, str);
        if (len > 24){
            send_string(cl, "name is 24 character max\n");
            return;
        }
        if (is_name_taken){
            send_string(cl, "name is taken\n");
            return;
        }
        cl->name = malloc(len + 1);
        strcpy(cl->name, str);
        cl->state = st_online;
        
        snprintf(buf, sizeof(buf), "%s entered the room\n", cl->name);
        broadcast_message(cs->sessions, buf);
}

void message_handling(struct chat_server *cs, struct client *cl, const char *str)
{
    char buf[2048];
    snprintf(buf, sizeof(buf), "%s: %s\n", cl->name, str);
    broadcast_message(cs->sessions, buf);
}

void command_users(struct chat_server *cs, struct client *cl)
{
    struct client_list *tmp;
    for (tmp = cs->sessions; tmp; tmp = tmp->next){
        send_string(cl, tmp->sess->name);
        send_string(cl, "\n");
    }
}

void command_quit(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    char buf[256];
    if (argc == 1)
        snprintf(buf, sizeof(buf), "%s left the room\n", cl->name);
    else
        snprintf(buf, sizeof(buf), "%s left the room with message: %s\n", cl->name, argv[1]);

    broadcast_message(cs->sessions, buf);
    cl->state = st_finished;
}

void command_private(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    char buf[256];
    if (argc != 3){
        send_string(cl, "use command: /private <nickname> \"<message>\"\n");
        return;
    }

    struct client *dest = find_client(cs, argv[1]);
    if (!dest){
        send_string(cl, "client not found\n");
        return;
    }
    add_to_private_names(cl, dest->name);
    cl->private_msgs_counter++;
    if (cl->private_msgs_counter > cs->max_private){
        cs->max_private = cl->private_msgs_counter;
        free(cs->max_private_name);
        cs->max_private_name = malloc(strlen(cl->name) + 1);
        strcpy(cs->max_private_name, cl->name);
    }
    snprintf(buf, sizeof(buf), "[private] %s: %s\n", cl->name, argv[2]);
    send_string(dest, buf);
}

void command_privates(struct client *cl)
{
    struct private_names *tmp;
    for (tmp = cl->p_names; tmp; tmp = tmp->next){
        send_string(cl, tmp->name);
        send_string(cl, "\n");
    }
}

void command_admin(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    if (argc < 2){
        send_string(cl, "no password provided\n");
        return;
    }
    if (!strcmp(cs->admin_password, argv[1])){
        cl->admin_rights = 1;
        send_string(cl, "you have got admin rights\n");
    }
    else {
        send_string(cl, "wrong password\n");
    }
}

void command_kick(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    char buf[1024];
    struct client *kicked_cl;
    if (argc < 2){
        send_string(cl, "use command: /kick <nickname> \"<message>\"\n");
        return;
    }
    if (!cl->admin_rights){
        send_string(cl, "you dont have such rights\n");
        return;
    }
    kicked_cl = find_client(cs, argv[1]);
    if (!kicked_cl){
        send_string(cl, "there is no such client\n");
        return;
    }
    if (kicked_cl->admin_rights){
        send_string(cl, "this user is administrator\n");
        return;
    }
    if (argc == 2){
        send_string(kicked_cl, "you were kicked\n");
    }
    else {
        snprintf(buf, sizeof(buf), "you were kicked for a reason: %s\n", argv[2]);
        send_string(kicked_cl, buf);
    }
    kicked_cl->state = st_finished;
}

void command_nick(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    char buf[1024];
    struct client *new_nick_cl;

    if (argc < 3){
        send_string(cl, "use command: /nick <nickname> <newnickname>\n");
        return;
    }

    new_nick_cl = find_client(cs, argv[1]);
    if (!new_nick_cl){
        send_string(cl, "user not found\n");
        return;
    }
    if ((!cl->admin_rights || new_nick_cl->admin_rights) && (cl != new_nick_cl)){
        send_string(cl, "you dont have such rights\n");
        return;
    }
    free(new_nick_cl->name);
    new_nick_cl->name = malloc(strlen(argv[2]) + 1);
    strcpy(new_nick_cl->name, argv[2]);
    snprintf(buf, sizeof(buf), "your name has been changed to: %s\n", argv[2]);
    send_string(new_nick_cl, buf);
}

void command_shutdown(struct chat_server *cs, struct client *cl, char **argv, int argc)
{
    char buf[1024];
    if (!cl->admin_rights){
        send_string(cl, "you dont have such rights\n");
        return;
    }
    if (argc > 1){
        snprintf(buf, sizeof(buf), "server closed with message: %s\n", argv[1]);
        broadcast_message(cs->sessions, buf);
    }
    else {
        broadcast_message(cs->sessions, "server closed\n");
    }
    server_down = 1;
}

void command_help(struct client *cl)
{
    send_string(cl,
        "/users\n"
        "/quit \"<message>\"\n"
        "/private <nickname> \"<message>\"\n"
        "/privates\n"
        "/help\n"
        "/kick <nickname> \"<message>\"\n"
        "/nick <nickname> <newnickname>\n"
        "/shutdown \"<message>\"\n"
        "/admin <password>\n"
    );
}

void handle_string(struct chat_server *cs, struct client *cl, const char *str)
{
    if (cl->state == st_wait_name){
        name_handling(cs, cl, str);
        return;
    }
    
    if (str[0] != '/'){
        message_handling(cs, cl, str);
        return;
    }
    struct token_list *tlist = read_tokens(str);
    if (!tlist) {
        send_string(cl, "command parsing failed\n");
        return;
    }
    
    if (!strcmp(tlist->argv[0], "/users"))
        command_users(cs, cl);
    else if (!strcmp(tlist->argv[0], "/quit"))
        command_quit(cs, cl, tlist->argv, tlist->used);
    else if (!strcmp(tlist->argv[0], "/private"))
        command_private(cs, cl, tlist->argv, tlist->used);
    else if (!strcmp(tlist->argv[0], "/privates"))
        command_privates(cl);
    else if (!strcmp(tlist->argv[0], "/help"))
        command_help(cl);
    else if (!strcmp(tlist->argv[0], "/kick"))
        command_kick(cs, cl, tlist->argv, tlist->used);
    else if (!strcmp(tlist->argv[0], "/nick"))
        command_nick(cs, cl, tlist->argv, tlist->used);
    else if (!strcmp(tlist->argv[0], "/shutdown"))
        command_shutdown(cs, cl, tlist->argv, tlist->used);
    else if (!strcmp(tlist->argv[0], "/admin"))
        command_admin(cs, cl, tlist->argv, tlist->used);
    else
        send_string(cl, "command not found\n");

    delete_token_list(tlist);
}

void handle_line_finish(struct chat_server *cs, struct client *cl)
{
    int i, pos;
    char *line;
    for (;;){
        pos = -1;
        for (i = 0; i < cl->buf_used; i++){
            if (cl->buf[i] == '\n'){
                pos = i;
                break;
            }
        }
        if (pos == -1)
            return;
        line = malloc(pos + 1);
        memcpy(line, cl->buf, pos);
        line[pos] = '\0';
        if (line[pos - 1] == '\r')
            line[pos - 1] = '\0';
        cl->buf_used -= (pos + 1);
        memmove(cl->buf, cl->buf + pos + 1, cl->buf_used);
        handle_string(cs, cl, line);
        free(line);
    }
}

void handle_client(struct chat_server *cs, struct client *cl)
{
    int rc = read(cl->sockfd, cl->buf + cl->buf_used, sizeof(cl->buf) - cl->buf_used);
    if (rc <= 0){
        cl->state = st_finished;
        return;
    }
    cl->buf_used += rc;
    handle_line_finish(cs, cl);
    if ((unsigned)cl->buf_used >= sizeof(cl->buf)){
        cl->buf_used = 0;
        send_string(cl, "line too long\n");
    }
}

void server_cycle(struct chat_server *cs)
{
    int res;
    fd_set readfds;
    struct client_list *tmp;
    while (!server_down){
        int max_d = cs->ls;
        FD_ZERO(&readfds);
        FD_SET(cs->ls, &readfds);
        for (tmp = cs->sessions; tmp; tmp = tmp->next){
            FD_SET(tmp->sess->sockfd, &readfds);
            if (tmp->sess->sockfd > max_d)
                max_d = tmp->sess->sockfd;
        }

        res = select(max_d + 1, &readfds, NULL, NULL, NULL);
        if (res < 0){
            if (errno != EINTR){
                perror("select");
                break;
            }
            continue;
        }

        if (FD_ISSET(cs->ls, &readfds))
            accept_client(cs);
        for (tmp = cs->sessions; tmp; tmp = tmp->next){
            if (FD_ISSET(tmp->sess->sockfd, &readfds))
                handle_client(cs, tmp->sess);
        }
        finish_sessions(&cs->sessions);
    }
}

struct chat_server *init_server(const char *ip, unsigned short port, char *password)
{
    int sockfd, ok, opt = 1;
    struct chat_server *cs;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ok = inet_aton(ip, &addr.sin_addr);
    if (!ok){
        perror("invalid address");
        return NULL;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket");
        return NULL;
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ok = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ok < 0){
        perror("bind");
        close(sockfd);
        return NULL;
    }
    ok = listen(sockfd, 16);
    if (ok < 0){
        perror("listen");
        close(sockfd);
        return NULL;
    }
    cs = malloc(sizeof(struct chat_server));
    cs->ls = sockfd;
    cs->sessions = NULL;
    cs->admin_password = password;
    cs->max_private = 0;
    cs->max_private_name = NULL;
    return cs;
}

void print_stat(struct chat_server *cs)
{
    if (cs->max_private_name) {
        printf("max private messages count: %d by %s\n",
            cs->max_private, cs->max_private_name);
    }
}

void destroy_server(struct chat_server *cs)
{
    shutdown(cs->ls, 2);
    close(cs->ls);
    delete_clients(cs->sessions);
    free(cs->max_private_name);
    free(cs);
}

int main(int argc, char **argv)
{
    struct chat_server *cs;
    if (argc != 4){
        fprintf(stderr, "bad arguments\n");
        return 1;
    }
    unsigned short port = atoi(argv[2]);
    cs = init_server(argv[1], port, argv[3]);
    if (!cs){
        fprintf(stderr, "fail to init server\n");
        return 1;
    }
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    server_cycle(cs);
    print_stat(cs);
    destroy_server(cs);
    return 0;
}

