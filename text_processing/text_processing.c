#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rand_malloc.h"

bool failedMemoryAllocation = false;

char *addTerminatingChar(char* line, size_t length, size_t* buffer) {
    if (length >= *buffer) {
        char* newbuffer;
        if (length < (size_t)-1) {
                *buffer = length + 1;
            }
            else {
                free(line);
                failedMemoryAllocation = true;
                return NULL;
            }
            newbuffer = realloc(line, *buffer);
            
            if (!newbuffer) {
                free(line);
                failedMemoryAllocation = true;
                return NULL;
            }
            line = newbuffer;
    }
    line[length++] = '\0';
    return line;
}

char *resizeBufferAndAddChar(char* line, size_t* length, size_t* buffer, int character) {
    char* newbuffer;
    if (*length >= *buffer) {
        if (*buffer == 0) {
            *buffer = 2;
        }
        else if(*buffer <= ((size_t)-1)/2) {
            *buffer = 2 * (*length);
        }
        else {
            free(line);
            failedMemoryAllocation = true;
            return NULL;
        }
        newbuffer = realloc(line, *buffer);
        if(!newbuffer) {
            free(line);
            failedMemoryAllocation = true;
            return NULL;
        }
        line = newbuffer;
    }
    line[(*length)++] = character;
    return line;
}

char *getLine() {

    char *line = NULL;
    size_t length = 0, buffer = 0;
    int character;

    while((character = getchar()) != EOF) {
        line = resizeBufferAndAddChar(line, &length, &buffer, character);
        
        if(failedMemoryAllocation) {
            return NULL;
        }

        if (character == '\n') {
            break;
        }
    }
        
    if((character == EOF) && (length == 0)) {
        return NULL;
    }

    line = addTerminatingChar(line, length, &buffer);
    if(failedMemoryAllocation) {
        return NULL;
    }
    return line;
}

char *reverseLine(char* line) {
    int len = strlen(line);
    char *reversedLine = malloc(len + 1); 
    if (!reversedLine) {
        failedMemoryAllocation = true;
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

 char** getText(int* counter, int* capacity)
 {
    char** lines = NULL;
    char* line;
    while ((line = getLine())) {
        if ((*counter) >= (*capacity)) {
            (*capacity) = ((*capacity) == 0) ? 2 : (*capacity) * 2;
            char **new_lines = realloc(lines, (*capacity) * sizeof(char*));
            if (!new_lines) {
                for(int i = 0; i < (*counter); i++) {
                    free(lines[i]);
                }
                free(line);
                free(lines);
                failedMemoryAllocation = true;
                exit(EXIT_FAILURE);
            }
            lines = new_lines;
        }
        lines[(*counter)++] = line;
    }
    return lines;
 }

void reverseLines(char** lines, int counter) {
    for (int i = counter; i > 0; i--) {
        char* reversedLine = reverseLine(lines[i-1]);
        if (!reversedLine) {
            for (int j = i-1; j >= 0; j--) {
                free(lines[j]);
            }
            free(lines);
            failedMemoryAllocation = true;
            exit(EXIT_FAILURE);
        }
        if(!failedMemoryAllocation) {     
        printf("%s\n", reversedLine);
        }
        free(reversedLine);
        free(lines[i - 1]);
    }
    free(lines);
}

int main()
{
    int counter = 0;
    int capacity = 0;
    char** text = getText(&counter, &capacity);
    reverseLines(text, counter);
}