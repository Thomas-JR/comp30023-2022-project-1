#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tools.h"

// Function to convert input file to graph
void processInput(char* fileName, file_t** files, int* filesLength, int* maxFiles) {
    unsigned int processId, fileId1, fileId2;
    FILE* file = fopen(fileName, "r");
    file_t* nextFile;

    // Iterate through each line of 3 inputs
    while (fscanf(file, "%d %d %d", &processId, &fileId1, &fileId2) != EOF) {
        // Create next file if it does not exist
        if (!filesContains(*files, *filesLength, fileId2)) {
            (*files)[*filesLength].processId = processId;
            (*files)[*filesLength].id = fileId2;
            (*files)[*filesLength].next = NULL;
            (*files)[*filesLength].locked = false;
            (*files)[*filesLength].hasNext = false;
            (*filesLength)++;
        }

        // Get the next file from the graph
        nextFile = getFile(*files, *filesLength, fileId2);

        if (filesContains(*files, *filesLength, fileId1)) {
            // Link existing process-file to the next file
            file_t* existing = getFile(*files, *filesLength, fileId1);
            existing->processId = processId;
            existing->id = fileId1;
            existing->next = nextFile;
            existing->hasNext = true;
        } else {
            // Create process-file in array, pointing to the next file
            (*files)[*filesLength].processId = processId;
            (*files)[*filesLength].id = fileId1;
            (*files)[*filesLength].next = nextFile;
            (*files)[*filesLength].hasNext = true;
            (*files)[*filesLength].locked = false;
            (*filesLength)++;
        }
    }
    
    fclose(file);
}

// Recursive dfs function to check for cycles, locking the nodes as we traverse
bool getCycleId(file_t* file, unsigned int startingFile, unsigned int* min) {
    *min = *min > file->processId ? file->processId : *min;

    // Return true if the current file is the file we started dfs from (cycle detected)
    if (file->id == startingFile) {
        return true;
    }

    // Return no cycle if DFS finishes before cycle detected
    if (file->locked || file->hasNext == false) {
        return false;
    }

    file->locked = true;
    return getCycleId(file->next, startingFile, min);
}

// Check if array of files contains the file id
bool filesContains(file_t* files, int filesLength, unsigned int id) {
    for (int i = 0; i < filesLength; i++) {
        if (files[i].id == id) {
            return true;
        }
    }
    return false;
}

// Check if array contains a value
bool arrayContains(int* arr, int n, int val) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == val) {
            return true;
        }
    }
    return false;
}

// Return a struct file_t from files if its id is matched
file_t* getFile(file_t* files, int filesLength, int id) {
    for (int i = 0; i < filesLength; i++) {
        if (files[i].id == id) {
            return &files[i];
        }
    }
    return NULL;
}

// Comparison function for integer array sorting
int intCmpFunc (const void * a, const void * b) {
    return (*(unsigned int*)a - *(unsigned int*)b);
}

// Comparison function for struct file_t array sorting based on process id
int fileCmpFunc (const void * a, const void * b) {
    file_t* fileA = (file_t*)a;
    file_t* fileB = (file_t*)b;
    return (fileA->processId - fileB->processId);
}

// Return the number of lines in a file
int countLines(char* fileName) {
    FILE* file = fopen(fileName, "r");
    int count = 0;
    char c;

    // Count newline characters
    for (c = getc(file); c != EOF; c = getc(file)) {
        count += c == '\n' ? 1 : 0;
    }

    fclose(file);
    return count;
}