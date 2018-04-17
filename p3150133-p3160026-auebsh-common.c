//
// Created by MrGoumX on 13/4/2018.
//
#include "p3150133-p3160026-auebsh.h"

int howMany(char *input) {
    char *temp = malloc(strlen(input) + 1);
    strcpy(temp, input);
    int count = 0;

    char *tk;
    tk = strtok(temp, " ");
    while (tk != NULL) {
        count++;
        tk = strtok(NULL, " ");
    }
    free(temp);
    return count;
}

int howManyCmd(char *input) {
    char *temp = malloc(strlen(input) + 1);
    strcpy(temp, input);
    int count = 0;

    char *tk;
    tk = strtok(temp, "|");
    while (tk != NULL) {
        count++;
        tk = strtok(NULL, "|");
    }
    free(temp);
    return count;
}

void saveArgs(char **args, char *input) {
    int count = 0;
    char *tk_pntr;

    char *tk;
    tk = strtok_r(input, " ", &tk_pntr);
    while (tk != NULL) {
        if (strlen(tk) != 0)
            args[count++] = tk;
        tk = strtok_r(NULL, " ", &tk_pntr);
    }
    args[count] = NULL;
}

void saveArgsWs(char **args, char *input){
    int count = 0;
    char *tk_pntr;

    char *tk;
    tk = strtok_r(input, " ", &tk_pntr);
    while(tk != NULL){
        if(strlen(tk) != 0 && *tk != '-'){
            args[count++] = tk;
        }
        tk = strtok_r(NULL, " ", &tk_pntr);
    }
    args[count] = NULL;
}

void redirect(int size, char **args, char **command) {
    int c = -1, cmdDone = 0, cmdCnt = 0;
    while (++c < size) {
        if (strcmp(args[c], ">") == 0) {
            freopen(args[++c], "w", stdout);
            cmdDone = 1;
        } else if (strcmp(args[c], ">>") == 0) {
            freopen(args[++c], "a+", stdout);
            cmdDone = 1;
        } else if (strcmp(args[c], "<") == 0) {
            freopen(args[++c], "r", stdin);
            cmdDone = 1;
        }
        if (cmdDone == 0) {
            command[c] = args[c];
            cmdCnt++;
        }
    }
    command[cmdCnt] = NULL;
}