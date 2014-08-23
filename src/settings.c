#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

void initSettings(Settings* settings, FileNames* fNames){

    FILE* fp = NULL;
    char buff[BUFSIZE];
    int value, cnt = 0;

    if((fp = fopen(fNames->cfgFileName, "r")) == NULL){ // default initialization
        printf("Error while opening cfg file (maybe there isn't one yet)\n");

        settings->language = ENG;
        settings->width = 300;
        settings->height = 450;

        return ;
    }
    else{ // get values from cfg file
        while(fscanf(fp, "%s = %d", buff, &value) == 2){
            switch(cnt){
                case LANG:
                    settings->language = value;
                    break;
                case WIDTH:
                    settings->width = value;
                    break;
                case HEIGHT:
                    settings->height = value;
                    break;
            }

            cnt++;
        }

        fclose(fp);

        return ;
    }
}

void dumpSettings(Settings* settings, FileNames* fNames){

    FILE* fp = NULL;

    fp = fopen(fNames->cfgFileName, "w");

    fprintf(fp, "language = %d\n", settings->language);
    fprintf(fp, "width = %d\n", settings->width);
    fprintf(fp, "height = %d", settings->height);

    fclose(fp);

    return ;
}
