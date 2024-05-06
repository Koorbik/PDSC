#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rand_malloc.h"

void handleAllocationError(char* line) {
    free(line);
    printf("Memory Allocation failed, exiting a program\n");
    exit(EXIT_FAILURE);
}

char *getLine() {

    char *line = NULL;
    size_t length = 0, buffer = 0;
    int character;

    while((character = getchar()) != EOF) {
        char *newbuffer;
        if (length >= buffer) {
            if (buffer == 0) {
                buffer = 2;
            }
            else if(buffer <= ((size_t)-1)/2) {
                buffer = 2*length;
            }
            else {
                handleAllocationError(line);
                return NULL;
            }
            newbuffer = (char*)realloc(line, buffer);
            if(!newbuffer) {
                handleAllocationError(line);
                return NULL;
            }
            line = newbuffer;
        }
        line[length++] = character;
        if (character == '\n') {
            break;
        }
    }

    if((character == EOF) && (length == 0)) {
        return NULL;
    }

    if (length >= buffer) {
        char* newbuffer;
        if (length < (size_t)-1) {
                buffer = length + 1;
            }
            else {
                handleAllocationError(line);
                return NULL;
            }
            newbuffer = (char*)realloc(line, buffer);
            
            if (!newbuffer) {
                handleAllocationError(line);
                return NULL;
            }
            line = newbuffer;
    }
    line[length++] = '\0'; 
    return line;
}

char *reverseLine(char* line) {
    int len = strlen(line);
    char *reversedLine = malloc(len + 1); 
    if (!reversedLine) {
        handleAllocationError(line);
        return NULL;
    }

    int i, j, index = 0, startIndex, endIndex = len - 1;
    for(i = len - 1; i >= 0; i--)
    {
        if(line[i] == ' ' || i == 0)
        {
            startIndex = (i == 0) ? i : i + 1;
            for(j = startIndex; j <= endIndex; j++)
            {
                if (line[j] != '\n') { 
                    reversedLine[index++] = line[j];
                }
            }
            if(i != 0) {
                reversedLine[index++] = ' ';
            }
            endIndex = i - 1;
        } 
    }
    reversedLine[index] = '\0';
    return reversedLine;
}

void reverseLines() {
    char** lines = NULL;
    int counter = 0;
    char* line;
    while ((line = getLine())) {
        char **new_lines = realloc(lines, (counter+1)* sizeof(char*));
        if (!new_lines) {
            free(line);
            free(lines);
            printf("Memory allocation failed, exiting the program\n");
            exit(EXIT_FAILURE);
        }
        lines = new_lines;
        lines[counter++] = line;
    }
    for (int i = counter; i > 0; i--) {
        char* reversedLine = reverseLine(lines[i-1]);
        printf("%s\n", reversedLine);
        free(reversedLine);
        free(lines[i - 1]);
    }
    free(lines);
    printf("\n");
}

int main()
{
    reverseLines();
    return 0;
}