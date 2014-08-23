#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markrow.h"

MarkRow* createMarkRow(int num, const char* name, int credits, int mark, int day, int month, int year){

    MarkRow* newRow = NULL;

    newRow = (MarkRow*) calloc(1, sizeof(MarkRow));

    newRow->num = num;
    newRow->name = strdup(name);
    newRow->credits = credits;
    newRow->mark = mark;
    newRow->day = day;
    newRow->month = month;
    newRow->year = year;
    newRow->date = (char*) calloc(11, sizeof(char)); // perfect fit for date (2 + 2 + 4 + 2 separators + 1 string terminator)
    //sprintf(newRow->date, "%02d/%02d/%04d", newRow->day, newRow->month, newRow->year);
    sprintf(newRow->date, "%04d/%02d/%02d", newRow->year, newRow->month, newRow->day);

    return newRow;
}

MarkRow* tokenizer(char* inputString, char* sep){

    MarkRow* mRow = NULL;
    char* token;
    int cnt = 0;

    mRow = (MarkRow*) calloc(1, sizeof(MarkRow));

    /* get the first token, containing row number */
    token = strtok(inputString, sep);
    mRow->num = atoi(token);
    cnt++;

    /* walk through other tokens */
    while(token != NULL){
        token = strtok(NULL, sep);

        switch(cnt){
            case 1:
                mRow->name = strdup(token);
                break;
            case 2:
                mRow->credits = atoi(token);
                break;
            case 3:
                mRow->mark = atoi(token);
                break;
            case 4:
                if(token[strlen(token) - 1] == '\n')
                    token[strlen(token) - 1] = '\0';
                sscanf(token, "%d/%d/%d", &mRow->day, &mRow->month, &mRow->year);
                mRow->date = strdup(token);
                break;
            default:
                break;
        }

        cnt++;
    }

    return mRow;
}

void freeMarkRow(MarkRow* markRow){

    free(markRow->name);
    free(markRow->date);
    free(markRow);

    return ;
}
