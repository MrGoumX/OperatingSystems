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
        int ann = checkForAnnexation(input);
        if (ann==-1) {
            printf("No annexations allowed\n");
            continue;
        }
        int toCheck = howMany(input);
        char *toCheckInput[toCheck];
        int sum = 0;
        int sop = howManyCmd(input);
        char *ptr;
        char* splitcmd = strtok_r(input, "|", &ptr);
        if (sop > 2) {
            printf("Only a single pipe is supported!\n");
            continue;
        }
        int pipe_p[2];
        if (sop > 1) {
            if(pipe(pipe_p) == -1) {
                perror("ERROR: Pipe failed.\n");
                exit(1);
            }
        }
        while (splitcmd != NULL) {
            int socmd = howMany(splitcmd);
            char* saveCmd[socmd];
            saveArgs(saveCmd, splitcmd);
            pid_t proc = fork();
            if (proc < 0) {
                perror("ERROR: Fork failed.\n");
                return -1;
            }
            if (proc == 0) {
                char *cmd[socmd];
                redirect(socmd, saveCmd, cmd);
                if (sop > 1) {
                    if (sum == 0) {
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
                if (sop > 1) {
                    if (sum == 0) {
                        close(pipe_p[1]);
                    } else {
                        close(pipe_p[0]);
                    }
                }
            }
            sum++;
            splitcmd = strtok_r(NULL, "|", &ptr);
        }
        free(input);
    }
}
