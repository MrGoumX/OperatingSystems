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
        printf("auebsh4> ");

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
        char *curr_ptr;
        char* curr_cmd = strtok_r(input, "|", &curr_ptr);

        if (cmd_total_len > 2) {
            printf("Only a single pipe is supported!\n");
            continue;
        }

        int pipe_p[2];
        if (cmd_total_len > 1) {
            if(pipe(pipe_p) == -1) {
                perror("ERROR: Pipe failed.\n");
                exit(1);
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

                if (cmd_total_len > 1) {
                    if (cmd_tick == 0) {
                        close(STDOUT_FILENO);
                        dup(pipe_p[1]);
                    } else {
                        close(STDIN_FILENO);
                        dup(pipe_p[0]);
                    }
                }

                execvp(cmd[0], cmd);
                exit(0);
            } else {
                int status;
                wait(&status);

                if (cmd_total_len > 1) {
                    if (cmd_tick == 0) {
                        close(pipe_p[1]);
                    } else {
                        close(pipe_p[0]);
                    }
                }
            }

            cmd_tick++;
            curr_cmd = strtok_r(NULL, "|", &curr_ptr);
        }

        free(input);
    }
}
