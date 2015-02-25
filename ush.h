#ifndef USH_H
#define USH_H

#define MAX_STRING_SIZE 100
#define MAX_COMMANDS    10

#include <string.h>

void getCommand(char *buffer);
void displayPrompt();
int parseCommand(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]);

#endif /* USH_H */

