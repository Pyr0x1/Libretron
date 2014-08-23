/*
 ============================================================================
 Name        : Libretron.c
 Author      : Loris "Pyrox" Gabriele
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "multistring.h"
#include "guistrings.h"
#include "gui.h"
#include "modelmanager.h"
#include "settings.h"
#include "filenames.h"

int main(void) {

	GuiStrings gStrings;
	GuiElems gElems;
	Settings settings;
	FileNames fNames;

	initGuiStrings(&gStrings);          // initializes strings to be displayed by gui
	initFileNames(&fNames);             // initializes data file and config file names
	initSettings(&settings, &fNames);   // initializes setting data structure
	initListStore(&gElems, &fNames);    // initializes data structures to display marks

    createGui(&gElems, &gStrings, &settings, &fNames);

    dumpSettings(&settings, &fNames);   // writes settings to file
    compactDataFile(&fNames);           // makes row numbers progressive eliminating "holes"

    freeGuiStrings(&gStrings);
    freeFileNames(&fNames);

	return EXIT_SUCCESS;
}
