#ifndef GUISTRINGS_H_
#define GUISTRINGS_H_

#include "multistring.h"

typedef struct guistrings{

	int nLangs;
	int nStrings;

	MultiString strings[20];

}GuiStrings;

int initGuiStrings(GuiStrings* gStrings);
void freeGuiStrings(GuiStrings* gStrings);

#endif /* STRINGS_H_ */
