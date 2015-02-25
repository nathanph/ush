#include <stdio.h>
#include "ush.h"

int main( int argc, const char* argv[] ) {

    char buffer[100];

    displayPrompt();
    getCommand( buffer, size );

}

void getCommand(char *buffer, size_t size) {
    fgets(buff, size, stdin);
}

void displayPrompt() {
    printf("ush> ");
}


