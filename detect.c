#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tasks.h"

int main(int argc, char* argv[]) {
    bool executionTime = false, filePerProcess = false;
    char* fileName = NULL;

    int i = 1;
    for (; i < argc; i++) {
        if (!strcmp(argv[i], "-f")) {
            fileName = argv[++i];
        } else if (!strcmp(argv[i], "-e")) {
            executionTime = true;
        } else if (!strcmp(argv[i], "-c")) {
            filePerProcess = true;
        }
    }

    if (fileName == NULL)
        return 0;
    
    // In worst case every line of the input has 2 unique files, make graph with up to this many files
    int maxFiles = countLines(fileName)*2, filesLength = 0;
    file_t* files = (file_t*)malloc(maxFiles*sizeof(file_t));

    // Converts the file into a graph
    processInput(fileName, &files, &filesLength, &maxFiles);

    if (!filePerProcess) {
        // Prints out # processes, # files
        task1(fileName, files, filesLength);
    }

    if (executionTime) {
        // Assumes no deadlock, prints execution time
        task2(fileName, files, &filesLength);
    } else if (!filePerProcess) {
        // Prints deadlock information
        task3(fileName, files, &filesLength);
    } else {
        // Constructs faster than sequential process processing
        task6(fileName, files, &filesLength);
    }

    free(files);

    return 0;
}