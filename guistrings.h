/*
 * guistrings.h
 *
 *  Created on: 14/ago/2014
 *      Author: loris
 */

#ifndef GUISTRINGS_H_
#define GUISTRINGS_H_

#include "multistring.h"

typedef struct guistrings{

	int nLangs;
	int nStrings;

	MultiString strings[18];

}GuiStrings;

int initGuiStrings(GuiStrings* gStrings);
void freeGuiStrings(GuiStrings* gStrings);

#endif /* STRINGS_H_ */
