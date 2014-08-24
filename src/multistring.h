#ifndef MULTISTRING_H_
#define MULTISTRING_H_

typedef struct multistring{

	int nLangs;
	char** multistring;

} MultiString;

int setMultiString(MultiString* mString, int nLangs, ...); // variable parameters are strings
void freeMultiString(MultiString* mString, int nLangs);


#endif /* MULTISTRING_H_ */
