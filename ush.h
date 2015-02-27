#ifndef USH_H
#define USH_H

#define MAX_STRING_SIZE 100
#define MAX_COMMANDS    10
#define MAX_ARGUMENTS   11
#define MAX_PIPES       2
#define PIPE_ONE        0
#define PIPE_TWO        1

#include <string.h>

void getCommand(char *buffer);
void displayPrompt();
int parseCommands(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]);
int parseArguments(char *buffer, char *arguments[MAX_ARGUMENTS]);
void forkShell( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );
void runCommands( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );
void print_hex( const char *string );
char * trim_whitespace( char *string ); 
void print_exec_args( char *arguments[MAX_ARGUMENTS], int argumentCount);

#endif /* USH_H */

