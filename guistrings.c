/*
 * guistrings.c
 *
 *  Created on: 14/ago/2014
 *      Author: loris
 */

#include <stdlib.h>
#include <stdio.h>
#include "guistrings.h"
#include "multistring.h"

int initGuiStrings(GuiStrings* gStrings){

	/* Number of Languages: 2 (EN, ITA) */
	/* Number of Strings: 1 */

	/* Strings are hardcoded in this section, give the correct "nLangs" value and
	 * write the correct number of localized strings for each MultiString */

	gStrings->nLangs = 2; // change this with the number of supported languages
	gStrings->nStrings = 15; // change this with the number of strings

	//gStrings->strings = (MultiString*) calloc(gStrings->nStrings, sizeof(MultiString));

	/* Declare all strings here */

	setMultiString(&(gStrings->strings[0]), gStrings->nLangs, "Edit", "Modifica");
	setMultiString(&(gStrings->strings[1]), gStrings->nLangs, "Help", "Aiuto");
	setMultiString(&(gStrings->strings[2]), gStrings->nLangs, "Quit", "Esci");
	setMultiString(&(gStrings->strings[3]), gStrings->nLangs, "About", "Informazioni");
	setMultiString(&(gStrings->strings[4]), gStrings->nLangs, "Language", "Lingua");
	setMultiString(&(gStrings->strings[5]), gStrings->nLangs, "Add", "Aggiungi");
	setMultiString(&(gStrings->strings[6]), gStrings->nLangs, "Calculate", "Calcola");
	setMultiString(&(gStrings->strings[7]), gStrings->nLangs, "Add element", "Aggiungi elemento");
	setMultiString(&(gStrings->strings[8]), gStrings->nLangs, "Cancel", "Annulla");
	setMultiString(&(gStrings->strings[9]), gStrings->nLangs, "Name", "Nome");
	setMultiString(&(gStrings->strings[10]), gStrings->nLangs, "Credits", "Crediti");
	setMultiString(&(gStrings->strings[11]), gStrings->nLangs, "Mark", "Voto");
	setMultiString(&(gStrings->strings[12]), gStrings->nLangs, "Date", "Data");
	setMultiString(&(gStrings->strings[13]), gStrings->nLangs, "Remove", "Elimina");
	setMultiString(&(gStrings->strings[14]), gStrings->nLangs, "English", "Inglese");
	setMultiString(&(gStrings->strings[15]), gStrings->nLangs, "Italian", "Italiano");
	setMultiString(&(gStrings->strings[16]), gStrings->nLangs, "Changes will be visible after restarting the application", "I cambiamenti saranno visibili dopo aver riavviato l'applicazione");
    setMultiString(&(gStrings->strings[17]), gStrings->nLangs, "Average", "Media");
    setMultiString(&(gStrings->strings[18]), gStrings->nLangs, "A simple electronic academic transcript", "Un semplice libretto universitario elettronico");

	return EXIT_SUCCESS;
}

void freeGuiStrings(GuiStrings* gStrings){

	int i;

	for(i = 0; i < gStrings->nStrings; i++)
		freeMultiString(&(gStrings->strings[i]), gStrings->nLangs);

	return ;
}


