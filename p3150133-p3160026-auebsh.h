//
// Created by MrGoumX on 13/4/2018.
//

#ifndef OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H
#define OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void populateArgs(char **args, char *input);
void populateArgsWs(char **args, char *input);
void populateCommandArray(char **args[], int* size, char *input);

int countArgs(char *input);
int countCommands(char *input);

void handleIORedirects(int size, char **args, char **command);

#endif //OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H
