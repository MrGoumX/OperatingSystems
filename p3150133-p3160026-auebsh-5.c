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

        if (strcmp(input, "exit") == 0) {
            printf("Bye bye!\n");
            exit(0);
        }

        int cmd_tick = 0;
        int cmd_total_len = countCommands(input);
        int pipe_count = cmd_total_len - 1;
        char *curr_ptr;
        char* curr_cmd = strtok_r(input, "|", &curr_ptr);

        int ** pipe_p;
        if (pipe_count) {
            pipe_p = malloc((pipe_count) * sizeof(int*));

            for (int i = 0; i < pipe_count; i++) {
                pipe_p[i] = malloc(2 * sizeof(int));
                if(pipe(pipe_p[i]) == -1) {
                    perror("ERROR: Pipe failed.\n");
                    exit(1);
                }
            }
        }

        while (curr_cmd != NULL) {
            int cmd_len = countArgs(curr_cmd);
            char* cmd_constr[cmd_len];

            populateArgs(cmd_constr, curr_cmd);

            pid_t pid = fork();

            if (pid < 0) {
                perror("ERROR: Fork failed.\n");
                return -1;
            }

            if (pid == 0) {
                char *cmd[cmd_len];
                handleIORedirects(cmd_len, cmd_constr, cmd);

                if (pipe_count) {
                    if (cmd_tick < pipe_count) {
                        close(STDOUT_FILENO);
                        dup(pipe_p[cmd_tick][1]);
                    }
                    if (cmd_tick > 0) {
                        close(STDIN_FILENO);
                        dup(pipe_p[cmd_tick - 1][0]);
                    }
                }

                execvp(cmd[0], cmd);
                exit(0);
            } else {
                int status;
                wait(&status);

                if (pipe_count) {
                    if (cmd_tick < pipe_count) {
                        close(pipe_p[cmd_tick][1]);
                    }
                    if (cmd_tick > 0) {
                        close(pipe_p[cmd_tick - 1][0]);
                    }
                }
            }

            cmd_tick++;
            curr_cmd = strtok_r(NULL, "|", &curr_ptr);
        }

        if (pipe_count)
            free(pipe_p);
        free(input);
    }
}
