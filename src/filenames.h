#ifndef FILENAMES_H_
#define FILENAMES_H_

typedef struct filenames{

    char *dataFileName;
	char *tmpFileName;
	char *cfgFileName;

}FileNames;

void initFileNames(FileNames* fNames);
void freeFileNames(FileNames* fNames);

#endif // FILENAMES_H_
