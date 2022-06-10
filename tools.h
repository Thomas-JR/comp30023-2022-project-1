#ifndef TOOLS_HEADER_FILE_H
#define TOOLS_HEADER_FILE_H

// Node structure for graph functions
typedef struct file_t {
    unsigned int processId;
    unsigned int id;
    struct file_t* next;
    bool locked, hasNext;
} file_t;

// Utility Functions
void processInput(char* fileName, file_t** files, int* filesLength, int* maxFiles);
bool getCycleId(file_t* file, unsigned int startingFile, unsigned int* min);
bool filesContains(file_t* files, int filesLength, unsigned int id);
bool arrayContains(int* arr, int n, int val);
file_t* getFile(file_t* files, int filesLength, int id);
int intCmpFunc (const void * a, const void * b);
int fileCmpFunc (const void * a, const void * b);
int countLines(char* fileName);

#endif