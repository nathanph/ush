#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ush.h"

int main( int argc, const char* argv[] ) {
    char buffer[MAX_STRING_SIZE];
    char commands[MAX_COMMANDS][MAX_STRING_SIZE];
    pid_t pid;

    while (true) {
        displayPrompt();
        getCommand( buffer );
        int commandCount = parseCommand( buffer, commands );
        
        forkShell( commands, commandCount );
    }
}

void getCommand(char *buffer) {
    fgets(buffer, MAX_STRING_SIZE, stdin);

    // Remove newline character
    for (int i=0; i<MAX_STRING_SIZE; i++)
    {
        if(buffer[i] == '\n')
        {
            buffer[i] = '\0';
        }
    }
}

void displayPrompt() {
    printf("ush> ");
}

int parseCommand(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]) {
    char *token;
    int commandCount=0;

    token = strtok(buffer, "|");

    while (token != NULL)
    {
        strcpy(commands[commandCount++], token);
        token = strtok(NULL, "|");
    }

    return commandCount;
}


void forkShell( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount ) {
    pid_t pid = fork();
    
    if (pid == -1)
    {
        // TODO:: Log borkedness.
    }

    // Child process.
    if (pid == 0) 
    {
        runCommands( commands, commandCount );
    }
    // Parent process.
    else if( pid > 0)
    {
        // Wait for child to end. 
        int returnStatus;
        waitpid(pid, &returnStatus, 0);
    }
}

void runCommands( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount ) {
    // TODO:: Piping stuff.
    printf("Commands running.\n");
    for(int i=0; i<commandCount; i++)
    {
        printf("%s\n", commands[i]);
        

    }
}
