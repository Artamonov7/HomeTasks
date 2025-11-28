#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "token_list.h"
#include <signal.h>

struct conveyor_elem{
    char **argv;
    struct conveyor_elem *next;
};

struct user_cmd{
    struct conveyor_elem *conv;
    char *infile;
    char *outfile;
    int append;
    int background;
};

void execute_conv(struct user_cmd *cmd)
{
    int fd[2], res, i, in_fd = -1, out_fd;
    int childs_count = 0;
    struct conveyor_elem *conv;
    for(conv = cmd->conv; conv; conv = conv->next){
        res = pipe(fd);
        if (res < 0){
            perror("pipe");
            exit(1);
        }
        res = fork();
        if (res < 0){
            perror("fork");
            exit(1);
        }
        if (res == 0){
            if (in_fd != -1){
                dup2(in_fd, 0);
                close(in_fd);
            }
            else if (cmd->infile){
                in_fd = open(cmd->infile, O_RDONLY);
                if (in_fd < 0){
                    perror(cmd->infile);
                    exit(1);
                }
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (conv->next){
                dup2(fd[1], 1);
            }
            else if (cmd->outfile){
                int flags = O_WRONLY | O_CREAT;
                if (cmd->append)
                    flags |= O_APPEND;
                else 
                    flags |= O_TRUNC;
                out_fd = open(cmd->outfile, flags, 0644);
                if (out_fd < 0){
                    perror(cmd->outfile);
                    exit(1);
                }
                dup2(out_fd, 1);
                close(out_fd);
            }
            close(fd[1]);
            close(fd[0]);
            execvp(conv->argv[0], conv->argv);
            perror("execvp");
            exit(1);
        }
        childs_count++;
        if (in_fd != -1)
            close(in_fd);
        in_fd = fd[0];
        close(fd[1]);
    }
    close(in_fd);
    if (!cmd->background){
        for (i = 0; i < childs_count; i++)
            wait(NULL);
    }
}

void execute_cmd(struct user_cmd *cmd)
{
    if (cmd->conv && !cmd->conv->next &&
        cmd->conv->argv[0] && !strcmp(cmd->conv->argv[0], "cd")){
        const char *path = cmd->conv->argv[1];
        if (!path)
            path = getenv("HOME");
        if (!path){
            fprintf(stderr, "HOME directory not set\n");
            return;
        }
        if (chdir(path) < 0){
            perror(path);
            return;
        } 
    }
    else {
       execute_conv(cmd);
    }
}

struct conveyor_elem *conv_append(struct conveyor_elem *first, int argc, char **argv)
{
    struct conveyor_elem *tmp;
    if (argc <= 0)
        return first;
    if (!first){
        first = malloc(sizeof(struct conveyor_elem));
        tmp = first;
    }
    else {
        tmp = first;
        while (tmp->next){
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(struct conveyor_elem));
        tmp = tmp->next;
    }
    tmp->argv = malloc((argc + 1)*sizeof(char *));
    int i;
    for (i = 0; i < argc; i++)
        tmp->argv[i] = argv[i];
    tmp->argv[argc] = NULL;
    tmp->next = NULL;
    return first;
}

struct user_cmd *parse_cmd(struct token_list *tlist)
{
    int i, pos = 0, pos2 = 0, pos2flag = 0;
    struct user_cmd *cmd = NULL;
    struct conveyor_elem *conv = NULL;
    if (!tlist->used)
        return NULL;

    cmd = malloc(sizeof(struct user_cmd));
    memset(cmd, 0, sizeof(struct user_cmd));

    if (!strcmp(tlist->argv[tlist->used - 1], "&"))
        cmd->background = 1;

    for (i = 0; i < tlist->used - cmd->background; i++){
        if (!strcmp(tlist->argv[i], "|")){
            conv = conv_append(conv, pos2 - pos, tlist->argv + pos);
            pos = i + 1;
            pos2 = i;
            pos2flag = 0;
        }
        else if (!strcmp(tlist->argv[i], "<") && (i != tlist->used - cmd->background - 1)){
            cmd->infile = tlist->argv[i + 1];
            pos2flag = 1;
        }
        else if (!strcmp(tlist->argv[i], ">") && (i != tlist->used - cmd->background - 1)){
            cmd->outfile = tlist->argv[i + 1];
            pos2flag = 1;
        }
        else if (!strcmp(tlist->argv[i], ">>") && (i != tlist->used - cmd->background - 1)){
            cmd->outfile = tlist->argv[i + 1];
            cmd->append = 1;
            pos2flag = 1;
        }
        if (!pos2flag)
            pos2++;
    }
    cmd->conv = conv_append(conv, pos2 - pos, tlist->argv + pos);
    return cmd;
}

void print_cmd(struct user_cmd *cmd)
{
    struct conveyor_elem *conv = cmd->conv;
    while (conv){
        int i;
        for(i = 0; conv->argv[i]; i++){
            fprintf(stderr, "%s ", conv->argv[i]);
        }
        fprintf(stderr, "\n---------------\n");
        conv = conv->next;
    }
    fprintf(stderr, "in: %s\nout: %s\nappend: %d\n", cmd->infile, cmd->outfile, cmd->append);
    fprintf(stderr, "background: %d\n", cmd->background);
}

void free_cmd(struct user_cmd *cmd)
{
    struct conveyor_elem *conv, *tmp;
    if (!cmd)
        return;
    conv = cmd->conv;
    while (conv){
        tmp = conv;
        conv = conv->next;
        free(tmp->argv);
        free(tmp);
    }
    free(cmd);
}

void welcome_user(void)
{
    char buf[256];
    if (getcwd(buf, sizeof(buf)))
        printf("%s$ ", buf);
    else
        printf("$ ");
    fflush(stdout);
}

void kill_zombie(int sig)
{
    while (wait4(-1, NULL, WNOHANG, NULL) > 0);
}

int main(void)
{
    struct token_list *tlist;
    struct user_cmd *cmd;
    signal(SIGCHLD, kill_zombie);
    while (!is_input_finished()){
        welcome_user();
        tlist = read_tokens();
        cmd = parse_cmd(tlist);
        if (cmd){
            print_cmd(cmd);
            execute_cmd(cmd);
        }
        free_cmd(cmd);
        delete_list(tlist);
    }
    return 0;
}

