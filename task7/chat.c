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

#define WAITING_FOR_NAME 0
#define ONLINE 1

struct client {
    int sockfd;
    char buf[1024];
    int buf_used;
    int finished;
    char *name;
    int status;
};

struct client_list {
    struct client *sess;
    struct client_list *next;
};

struct chat_server {
    struct client_list *sessions;
    int ls;
};

void add_client(struct client_list **list, struct client *cl)
{
    struct client_list *head = malloc(sizeof(struct client_list));
    head->sess = cl;
    head->next = *list;
    *list = head;
}

void delete_client(struct client *cl)
{
    shutdown(cl->sockfd, 2);
    close(cl->sockfd);
    free(cl);
}

void finish_sessions(struct client_list **list)
{
    while (*list){
        if ((*list)->sess->finished){
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
    cl->sockfd = sock;
    cl->status = WAITING_FOR_NAME;
    add_client(&cs->sessions, cl);
    send_string(cl, "Hello, enter your name: ");
    fprintf(stderr, "connection from %s:%u\n",
        inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

void handle_string(struct chat_server *cs, struct client *cl, const char *str)
{
    int i, j;
    if (cl->status == WAITING_FOR_NAME){
        cl->name = malloc(strlen(str) + 1);
        strcpy(cl->name, str);
        cl->status = ONLINE;

        const char welcome[] = " entered the room!\n";
        int length = strlen(cl->name) + strlen(welcome) + 1;
        char *mesg = malloc(length);
        for (i = 0; i < strlen(cl->name); i++)
            mesg[i] = cl->name[i];
        for (j = 0; j < strlen(welcome); j++)
            mesg[i + j] = welcome[j];
        mesg[i + j] = '\0';

        broadcast_message(cs->sessions, mesg);
        free(mesg);
        return;
    }

    if (!strcmp(str, "/users")){
        struct client_list *tmp;
        for (tmp = cs->sessions; tmp; tmp = tmp->next){
            send_string(cl, tmp->sess->name);
            send_string(cl, "\n");
        }
        return;
    }
    
    char *checkquit = malloc(strlen("/quit ") + 1);
    for (i = 0; i < strlen("/quit "); i++)
        checkquit[i] = str[i];
    checkquit[i] = '\0';

    if (!strcmp(checkquit, "/quit ")){
        const char goodbye[] = " left the room with message: ";
        int length = strlen(cl->name) + strlen(goodbye) + strlen(str) - strlen("/quit ") + 3;
        char *mesg = malloc(length);
        for (i = 0; i < strlen(cl->name); i++)
            mesg[i] = cl->name[i];
        for (j = 0; j < strlen(goodbye); j++)
            mesg[i + j] = goodbye[j];
        int k;
        for (k = 0; k < strlen(str) - 6; k++){
            mesg[i + j + k] = str[k + 6];
        }
        mesg[i + j + k] = '\n';
        mesg[i + j + k + 1] = '\0';

        broadcast_message(cs->sessions, mesg);
        free(mesg);
        cl->finished = 1;
        free(checkquit);
        return;
    }
    free(checkquit);

    int length = strlen(cl->name) + strlen(str) + 4;
    char *mesg = malloc(length);
    
    for (i = 0; i < strlen(cl->name); i++)
        mesg[i] = cl->name[i];
    mesg[i] = ':';
    mesg[i + 1] = ' ';
    for (j = 0; j < strlen(str); j++)
        mesg[i + 2 + j] = str[j];
    mesg[i + 2 + j] = '\n';
    mesg[i + 2 + j + 1] = '\0';
    
    broadcast_message(cs->sessions, mesg);
    free(mesg);
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
        cl->finished = 1;
        return;
    }
    cl->buf_used += rc;
    handle_line_finish(cs, cl);
    if (cl->buf_used >= sizeof(cl->buf)){
        cl->buf_used = 0;
        send_string(cl, "line too long\n");
    }
}

void server_cycle(struct chat_server *cs)
{
    int res;
    fd_set readfds;
    struct client_list *tmp;
    for (;;){
        int max_d = cs->ls;
        FD_ZERO(&readfds);
        FD_SET(cs->ls, &readfds);
        for (tmp = cs->sessions; tmp; tmp = tmp->next){
            FD_SET(tmp->sess->sockfd, &readfds);
            if (tmp->sess->sockfd > max_d){
                max_d = tmp->sess->sockfd;
            }
        }
        res = select(max_d + 1, &readfds, NULL, NULL, NULL);
        if (res < 0){
            if (errno != EINTR){
                perror("select");
                break;
            }
            continue;
        }
        if (FD_ISSET(cs->ls, &readfds)){
            accept_client(cs);
        }
        for (tmp = cs->sessions; tmp; tmp = tmp->next){
            if (FD_ISSET(tmp->sess->sockfd, &readfds)){
                handle_client(cs, tmp->sess);
            }
        }
        finish_sessions(&cs->sessions);
    }
}

struct chat_server *init_server(const char *ip, unsigned short port)
{
    int sockfd, ok;
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
    return cs;
}

void destroy_server(struct chat_server *cs)
{
    shutdown(cs->ls, 2);
    close(cs->ls);
    free(cs);
}

int main(int argc, char **argv)
{
    struct chat_server *cs;
    if (argc != 3){
        fprintf(stderr, "bad arguments\n");
        return 1;
    }
    unsigned short port = atoi(argv[2]);
    cs = init_server(argv[1], port);
    if (!cs){
        fprintf(stderr, "fail to init server\n");
        return 1;
    }
    server_cycle(cs);
    destroy_server(cs);
    return 0;
}

