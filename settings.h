#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "filenames.h"

#define BUFSIZE 256

enum{ ENG, ITA };

enum{ LANG, WIDTH, HEIGHT };

typedef struct settings{

    int language;
    int width;
    int height;

}Settings;

void initSettings(Settings* settings, FileNames* fNames);
void dumpSettings(Settings* settings, FileNames* fNames);

#endif // SETTINGS_H_
