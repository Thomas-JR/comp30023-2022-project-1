#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "tasks.h"

void task1(char* fileName, file_t* files, int filesLength) {
    // Store list of processes
    unsigned int* processes = (unsigned int*)malloc(filesLength*sizeof(int));
    int processesLength = 0;

    for (int i = 0; i < filesLength; i++) {
        processes[processesLength++] = files[i].processId;
    }

    // Sort the processes to be able to quickly remove duplicates
    qsort(processes, processesLength, sizeof(int), intCmpFunc);

    // Remove Duplicates
    int i = 0, j = 1;
    while (j < filesLength) {
        if (processes[j] != processes[j-1])
            i++;
        processes[i] = processes[j++];
    }
    processesLength = i+1;
    
    printf("Processes %d\n", processesLength);
    printf("Files %d\n", filesLength);
    free(processes);
}

void task2(char* fileName, file_t* files, int* filesLength) {
    // Store the requested files
    unsigned int* targets = (unsigned int*)malloc(*filesLength*sizeof(int));
    int targetsLength = 0;
    
    // Add all requested files to array
    for (int i = 0; i < *filesLength; i++) {
        if (files[i].next != NULL) {
            targets[targetsLength++] = files[i].next->id;
        }
    }

    // Sort the requested files for quicker longest-repeating-subsequence retrieval
    qsort(targets, targetsLength, sizeof(int), intCmpFunc);

    // Get longest-repeating-subsequence
    int best = 0, cur = 1;
    for (int i = 1; i < targetsLength; i++) {
        if (targets[i] != targets[i-1]) {
            best = cur > best ? cur : best;
            cur = 0;
        }
        cur++;
    }

    best = cur > best ? cur : best;

    // Due to parallelisation, the longest time it would take to run all processes is k+1
    printf("Execution time %d\n", best+1);
    free(targets);
}

void task3(char* fileName, file_t* files, int* filesLength) {
    unsigned int terminatedProcess;
    unsigned int* terminatedProcesses = (unsigned int*)malloc((*filesLength)*sizeof(int));
    int terminatedProcessesLength = 0;

    // Iterate through every node, checking whether DFS from this node ever returns to this node
    // O(n^2) could be improved on by creating a visited set with O(1) lookup -> O(n)
    for (int i = 0; i < *filesLength; i++) {
        terminatedProcess = UINT_MAX;
        // If a cycle is detected returning to the starting file, terminate the min process id in this cycle
        if (files[i].hasNext && getCycleId(files[i].next, files[i].id, &terminatedProcess)) {
            if (terminatedProcessesLength == 0 || terminatedProcesses[terminatedProcessesLength-1] != terminatedProcess) {
                terminatedProcesses[terminatedProcessesLength++] = terminatedProcess;
            }
        }
        // Reset locks on all files
        for (int j = 0; j < *filesLength; j++) {
            files[j].locked = false;
        }
    }

    // Print expected output
    if (terminatedProcessesLength > 0) {
        printf("Deadlock detected\nTerminate");
        for (int i = 0; i < terminatedProcessesLength; i++) {
            printf(" %d", terminatedProcesses[i]);
        }
        printf("\n");

        free(terminatedProcesses);

        return;
    }

    printf("No deadlocks\n");
    free(terminatedProcesses);
}

void task6(char* fileName, file_t* files, int* filesLength) {
    // Keep track of processes already performed
    int* removedFiles = (int*)malloc(*filesLength*sizeof(int));
    int removedFilesLength = 0;
    int iteration = 0;
    bool change;

    // Each while loop iteration is one iteration of simulation time
    while (true) {
        change = false;
        // Iterate through all processes, removing unlocked file pairs
        for (int i = 0; i < *filesLength; i++) {
            if (files[i].locked || files[i].next == NULL || files[i].next->locked 
                || arrayContains(removedFiles, removedFilesLength, files[i].processId)) {
                continue;
            }
            
            // Found remaining process with 2 unlocked files
            printf("%d %d %d,%d\n", iteration, files[i].processId, files[i].id, files[i].next->id);
            files[i].locked = true;
            files[i].next->locked = true;
            removedFiles[removedFilesLength++] = files[i].processId;
            change = true;
        }

        // Reset all locks
        for (int i = 0; i < *filesLength; i++) {
            files[i].locked = false;
        }

        iteration++;

        // Break if there are no processes left
        if (!change) {
            break;
        }
    }
    
    printf("Simulation time %d\n", iteration-1);

    free(removedFiles);

    return;
}