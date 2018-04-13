#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int length = 20;
    char *input;
    char c;

    while (1) {
        printf("auebsh1> ");

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

        pid_t pid = fork();

        if (pid < 0) {
            perror("ERROR: Fork failed.\n");
            return -1;
        }

        if (pid == 0) {
            char *args[2];
            args[0] = input;
            args[1] = NULL;

            execvp(input, args);
            exit(0);
        }
        else {
            int status;
            int wpid = wait(&status);

            if (wpid == -1) {
                perror("ERROR: Waitpid failed.\n");
                return -1;
            }
        }
        free(input);
    }
}