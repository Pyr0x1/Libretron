#ifndef GUI_H_
#define GUI_H_

#define BUFSIZE 256

#include <gtk/gtk.h>
#include "guistrings.h"
#include "markrow.h"
#include "settings.h"
#include "filenames.h"

// Enum type for columns of list
enum{

    COL_NUM,
    COL_NAME,
    COL_CREDITS,
    COL_MARK,
    COL_DATE,
    COL_TOT

};

typedef struct guielems{

	GtkWidget *window;
	GtkWidget *menuBar;
	GtkWidget *fileMenu;
	GtkWidget *editMenu;
	GtkWidget *helpMenu;
	GtkWidget *fileMenuLabel;
	GtkWidget *editMenuLabel;
	GtkWidget *helpMenuLabel;
	GtkWidget *exitMenuLabel;
	GtkWidget *infoMenuLabel;
	GtkWidget *langMenuLabel;
	GtkWidget *itaMenuLabel;
	GtkWidget *engMenuLabel;
	GtkWidget *chooseLangMenu;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *scrollWindow;
	GtkWidget *addButton;
	GtkWidget *removeButton;
	GtkWidget *calcButton;
	GtkWidget *resultLabel;
	GtkWidget *gradLabel;

	GdkPixbuf *icon;

    GtkWidget *view;
	GtkListStore *store;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	int numElems;
	int newRowNumber;

}GuiElems;

typedef struct param{

    GuiElems* gElems;
    GuiStrings* gStrings;
    FileNames* fNames;
    Settings* settings;
    int curLan;

}GuiParam;

void createGui(GuiElems* gElems, GuiStrings* gStrings, Settings* settings, FileNames* fNames);
void createMenu(GuiParam* data);
void showPopup(GtkButton *addButton, gpointer data);
void removeHandler(GtkButton *removeButton, gpointer data);
void updateWinSize(GtkWidget *window, gpointer data);
void changeLanguage(GtkMenuItem *langMenuLabel, gpointer data);
void showAboutPopup(GtkMenuItem *infoMenuLabel, gpointer data);
void calcAvg(GuiElems* gElems, GuiStrings* gStrings, int curLan);

#endif /* GUI_H_ */
