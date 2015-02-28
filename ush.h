#ifndef USH_H
#define USH_H

#define MAX_STRING_SIZE 100
#define MAX_COMMANDS    10
#define MAX_ARGUMENTS   11
#define PIPE_OUT        0
#define PIPE_IN         1
#define PIPE_SIZE       2
#define STD_IN          0
#define STD_OUT         1

#include <string.h>

void getCommand(char *buffer);
void displayPrompt();
int parseCommands(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]);
int parseArguments(char *buffer, char *arguments[MAX_ARGUMENTS]);
void forkShell( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );
void runCommands( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount );
void print_hex( const char *string );
char * trim_whitespace( char *string ); 
void print_exec_args( char *arguments[MAX_ARGUMENTS], int argumentCount );
void open_pipes( int fd[][PIPE_SIZE], int commandCount );
void close_pipes( int fd[][PIPE_SIZE], int commandCount );
void print_pipes( int fd[][PIPE_SIZE], int commandCount );

#endif /* USH_H */

