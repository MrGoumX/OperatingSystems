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
        printf("auebsh2> ");
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
        int args = howMany(input);
        char* toExec[args];
        saveArgsWs(toExec, input);
        pid_t proc = fork();
        if (proc < 0) {
            perror("ERROR: Fork failed.\n");
            return -1;
        }
        if (proc == 0) {
            char *cmd[args];
            redirect(args, toExec, cmd);
            execvp(cmd[0], cmd);
            exit(0);
        } else {
            int status;
            int wproc = wait(&status);
            if (wproc == -1) {
                perror("ERROR: Waitpid failed.\n");
                return -1;
            }
        }
        free(input);
    }
}
