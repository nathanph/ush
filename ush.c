#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
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
    // printf("Commands running.\n");
    
    int fd[commandCount-1][2];
    int firstPid=0;

    open_pipes( fd , commandCount );

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
            if(commandCount != 1)
            {
                if(i==0)
                {
                    // Set standard out to write to pipe for next command.
                    dup2(fd[i][PIPE_IN], STD_OUT);
                    close(fd[i][PIPE_OUT]);
                }
                else if(i==commandCount-1)
                {
                    exit(EXIT_FAILURE);
                    // Set standard in to read from pipe out of previous command.
                    dup2(fd[i-1][PIPE_OUT], STD_IN);
                    close(fd[i-1][PIPE_IN]);
                }
                else
                {
                    // Set standard in to read from pipe out of previous command.
                    dup2(fd[i-1][PIPE_OUT], STD_IN);
                    close(fd[i-1][PIPE_IN]);
                    // Set standard out to write to pipe for next command.
                    dup2(fd[i][PIPE_IN], STD_OUT);
                    close(fd[i][PIPE_OUT]);
                }
            }

            // print_exec_args( arguments, argumentCount );
            
            execvp(commands[i], arguments);
            perror("execvp");
        }
        else if (pid > 0)
        {
            if(firstPid==0)
            {
                firstPid=pid;
            }

            if(i==commandCount-1)
            {
                // Set standard in to read from pipe out of previous command.
                dup2(fd[i-1][PIPE_OUT], STD_IN);
                close(fd[i-1][PIPE_IN]);
                setpgid( 0, firstPid);

                execvp(commands[i], arguments);
                perror("execvp");
            }

            setpgid( pid, firstPid);
//            int returnStatus;
//            waitpid(pid, &returnStatus, 0);
        }
    }

    close_pipes( fd, commandCount);

    // Wait for all children processes to exit.
    while (wait(NULL) > 0) ;
}

void open_pipes( int fd[][PIPE_SIZE], int commandCount ) {
    for(int i=0; i<commandCount-1; i++)
    {
        pipe(fd[i]);
    }
}

void close_pipes( int fd[][PIPE_SIZE], int commandCount ) {
    for(int i=0; i<commandCount-1; i++)
    {
        close(fd[i][PIPE_OUT]);
        close(fd[i][PIPE_IN]);
    }
}

void print_pipes( int fd[][PIPE_SIZE], int commandCount ) {
    for(int i=0; i<commandCount-1; i++) 
    {
        printf("Pipe %i:\n", i);
        printf(" -PIPE_IN:  %i\n", fd[i][PIPE_IN]);
        printf(" -PIPE_OUT: %i\n", fd[i][PIPE_OUT]);
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
