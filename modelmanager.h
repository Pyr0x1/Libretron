#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include "gui.h"
#include "filenames.h"

int initListStore(GuiElems* gElems, FileNames* fNames);
int deleteElemFromListStore(GuiElems* gElems);
void compactDataFile(FileNames* fNames);

#endif /* MODELMANAGER_H_ */
