/*
 * multistring.c
 *
 *  Created on: 14/ago/2014
 *      Author: loris
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "multistring.h"

int setMultiString(MultiString* mString, int nLangs, ...){

	va_list arguments;
	char* string;
	int i = 0;
	int retVal = EXIT_SUCCESS;

	if(nLangs > 0){

		va_start(arguments, nLangs);
		mString->multistring = (char**) calloc(nLangs, sizeof(char*));

		for(i = 0; i < nLangs; i++){
			string = va_arg(arguments, char*);

			if(string == NULL){
				printf("Warning: number of strings different from specified number of languages\n");
				fflush(stdout);
				retVal = EXIT_FAILURE; // wrong parameters
				break;
			}

			mString->multistring[i] = strdup(string);
		}

		va_end(arguments); // always performed even in case of wrong parameters
	}
	else // if number of languages is less than 1 return failure
		retVal = EXIT_FAILURE;

	return retVal;
}

void freeMultiString(MultiString* mString, int nLangs){

	int i;

	for(i = 0; i < nLangs; i++)
		free(mString->multistring[i]);

	free(mString->multistring);

	return ;
}
