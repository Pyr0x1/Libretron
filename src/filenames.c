#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filenames.h"

void initFileNames(FileNames* fNames){

    char* homeDir = getenv("HOME");
    int dataFileLen = 0;

    dataFileLen += strlen(homeDir);
    dataFileLen += strlen("/.libretron/data");
    dataFileLen++; // string terminator

    fNames->dataFileName = (char*) calloc(dataFileLen, sizeof(char));
    strcat(fNames->dataFileName, homeDir);
    strcat(fNames->dataFileName, "/.libretron/data");

    dataFileLen = 0;

    dataFileLen += strlen(homeDir);
    dataFileLen += strlen("/.libretron/tmp");
    dataFileLen++; // string terminator

    fNames->tmpFileName = (char*) calloc(dataFileLen, sizeof(char));
    strcat(fNames->tmpFileName, homeDir);
    strcat(fNames->tmpFileName, "/.libretron/tmp");

    dataFileLen = 0;

    dataFileLen += strlen(homeDir);
    dataFileLen += strlen("/.libretron/cfg");
    dataFileLen++; // string terminator

    fNames->cfgFileName = (char*) calloc(dataFileLen, sizeof(char));
    strcat(fNames->cfgFileName, homeDir);
    strcat(fNames->cfgFileName, "/.libretron/cfg");

    return ;
}

void freeFileNames(FileNames* fNames){

    free(fNames->dataFileName);
    free(fNames->tmpFileName);
    free(fNames->cfgFileName);

    return ;
}
