#ifndef OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H
#define OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void saveArgs(char **args, char *input);
void saveArgsWs(char **args, char *input);

int howMany(char *input);
int howManyCmd(char *input);
int checkForAnnexation(char *args);

void redirect(int size, char **args, char **command);

#endif //OPERATINGSYSTEMS_P3150133_P3160026_AUEBSH_H
