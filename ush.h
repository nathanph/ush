#ifndef USH_H
#define USH_H

#define MAX_STRING_SIZE 100
#define MAX_COMMANDS    10

#include <string.h>

void getCommand(char *buffer);
void displayPrompt();
int parseCommand(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]);
void forkShell( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );
void runCommands( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );

#endif /* USH_H */

