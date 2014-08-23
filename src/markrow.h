#ifndef MARKROW_H_INCLUDED
#define MARKROW_H_INCLUDED

typedef struct markrow{

    unsigned int num;
    char* name;
    unsigned int credits;
    unsigned int mark;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    char* date;

}MarkRow;

MarkRow* createMarkRow(int num, const char* name, int credits, int mark, int day, int month, int year); // returned value must be freed
MarkRow* tokenizer(char* inputString, char* sep); // returned value must be freed
void freeMarkRow(MarkRow* markRow);

#endif /* MARKROW_H_ */
