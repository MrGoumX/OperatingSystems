#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "p3150133-p3160026-auebsh.h"

int main() {
    int length = 20;
    char *input;
    char c;

    while (1) {
        printf("auebsh5> ");
        input = malloc(length * sizeof(char));
        int count = 0;
        while((c = getchar()) != '\n' && c != EOF){
            if(count >= length){
                input = realloc(input, (length += 10) * sizeof(char));
            }
            input[count++] = c;
        }
        input[count]= '\0';
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            exit(0);
        }
        int sum = 0;
        int sop = howManyCmd(input);
        int sop2 = sop - 1;
        char *ptr;
        char* splitcmd = strtok_r(input, "|", &ptr);
        int ** pipe_p;
        if (sop2) {
            pipe_p = malloc((sop2) * sizeof(int*));
            for (int i = 0; i < sop2; i++) {
                pipe_p[i] = malloc(2 * sizeof(int));
                if(pipe(pipe_p[i]) == -1) {
                    perror("ERROR: Pipe failed.\n");
                    exit(1);
                }
            }
        }
        while (splitcmd != NULL) {
            int socmd = howMany(splitcmd);
            char* cmd_constr[socmd];
            saveArgs(cmd_constr, splitcmd);
            pid_t proc = fork();
            if (proc < 0) {
                perror("ERROR: Fork failed.\n");
                return -1;
            }
            if (proc == 0) {
                char *cmd[socmd];
                redirect(socmd, cmd_constr, cmd);
                if (sop2) {
                    if (sum < sop2) {
                        close(STDOUT_FILENO);
                        dup(pipe_p[sum][1]);
                    }
                    if (sum > 0) {
                        close(STDIN_FILENO);
                        dup(pipe_p[sum - 1][0]);
                    }
                }
                execvp(cmd[0], cmd);
                exit(0);
            } else {
                int status;
                wait(&status);
                if (sop2) {
                    if (sum < sop2) {
                        close(pipe_p[sum][1]);
                    }
                    if (sum > 0) {
                        close(pipe_p[sum - 1][0]);
                    }
                }
            }
            sum++;
            splitcmd = strtok_r(NULL, "|", &ptr);
        }
        if (sop2)
            free(pipe_p);
        free(input);
    }
}
