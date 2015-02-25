#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "ush.h"

int main( int argc, const char* argv[] ) {
    char buffer[MAX_STRING_SIZE];
    char commands[MAX_COMMANDS][MAX_STRING_SIZE];

    while (true) {
        displayPrompt();
        getCommand( buffer );
        parseCommand( buffer, commands );
    
//        for(int i=0;i<commandCount; i++)
//        {
//            printf("%s\n", commands[i]);
//        }
    
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


