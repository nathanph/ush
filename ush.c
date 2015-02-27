#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "ush.h"

int main( int argc, const char* argv[] ) {
    char buffer[MAX_STRING_SIZE];
    char commands[MAX_COMMANDS][MAX_STRING_SIZE];

    while (true) {
        displayPrompt();
        getCommand( buffer );
        int commandCount = parseCommands( buffer, commands );

        // printf("Buffer: %s\n", buffer);

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

int parseCommands(char *buffer, char commands[MAX_COMMANDS][MAX_STRING_SIZE]) {
    char *token;
    int commandCount=0;

    token = strtok(buffer, "|");

    while (token != NULL)
    {
        token = trim_whitespace(token);
        strcpy(commands[commandCount++], token);
        token = strtok(NULL, "|");
    }

    return commandCount;
}

int parseArguments(char *buffer, char *arguments[MAX_ARGUMENTS]) {
    char *token;
    int argumentCount=0;

    token = strtok(buffer, " ");

    while (token != NULL)
    {
        arguments[argumentCount++] = token;

        token = strtok(NULL, " ");
    }

    arguments[argumentCount] = NULL;
    
    return argumentCount;
}

void forkShell( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount) {
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

void runCommands( char commands[MAX_COMMANDS][MAX_STRING_SIZE], int commandCount) {
    // TODO:: Piping stuff.
    printf("Commands running.\n");
    

    int fd[MAX_PIPES][2]; 

    pipe(fd[PIPE_ONE]);
    pipe(fd[PIPE_TWO]);

    

    for(int i=0; i<commandCount; i++)
    {
        char *arguments[MAX_ARGUMENTS];
        int argumentCount = parseArguments( commands[i], arguments );

        pid_t pid = fork();

        if (pid == -1)
        {
            // TODO:: Log borkedness.
        }
        if (pid == 0)
        {
            print_exec_args( arguments, argumentCount );
            
            execvp(commands[i], arguments);
            perror("execvp");
        }
        else if (pid > 0)
        {
            int returnStatus;
            waitpid(pid, &returnStatus, 0);
        }
    }
}

void print_exec_args(char *arguments[MAX_ARGUMENTS], int argumentCount) {
    printf("Executing: %s\n", arguments[0]);
    printf("Arguments: ");
    for (int i=0; i<argumentCount; i++)
    {
        printf("%s ", arguments[i]);
    }
    printf("\n");
}

void print_hex(const char *s)
{
    while(*s)
        printf("%02x", (unsigned int) *s++);
    printf("%02x", (unsigned int) *s++);

    printf("\n");
}

char * trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while(isspace(*str)) str++;

    if(*str == 0)  // All spaces?
    return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}
