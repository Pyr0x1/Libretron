#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "markrow.h"
#include "modelmanager.h"
#include "settings.h"

void createGui(GuiElems* gElems, GuiStrings* gStrings, Settings* settings, FileNames* fNames){

    int curLan = settings->language; // current language = 0 (EN) change this to config file
    GuiParam gParam;

    gtk_init (0, NULL);

    // Initialize Gtk
    gElems->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (gElems->window), "Libretron");
    gtk_window_set_default_size(GTK_WINDOW(gElems->window), settings->width, settings->height);
    gtk_window_set_position(GTK_WINDOW(gElems->window), GTK_WIN_POS_CENTER);
    g_signal_connect (gElems->window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    // Load icon
    gElems->icon = gdk_pixbuf_new_from_file("/usr/share/pixmaps/libretron.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(gElems->window), gElems->icon);
    gtk_window_set_default_icon(gElems->icon);

    // Create vertical box to contain elements
    gElems->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add (GTK_CONTAINER (gElems->window), gElems->vbox);

    // Structure used to pass gui elements and multilanguage strings to handlers
    gParam.gElems = gElems;
    gParam.gStrings = gStrings;
    gParam.fNames = fNames;
    gParam.settings = settings;
    gParam.curLan = curLan;

    // Create menu
    createMenu(&gParam);

    // Create scrolled window to host marks
    gElems->scrollWindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (gElems->scrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_box_pack_start (GTK_BOX (gElems->vbox), gElems->scrollWindow, TRUE, TRUE, 0);

    // Create view for the model
    gElems->view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (gElems->store));
    g_object_unref(G_OBJECT(gElems->store));

    gElems->renderer = gtk_cell_renderer_text_new ();
    gElems->column = gtk_tree_view_column_new_with_attributes (gStrings->strings[9].multistring[curLan],
                                                               gElems->renderer,
                                                               "text", COL_NAME,
                                                               NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (gElems->view), gElems->column);
    gtk_tree_view_column_set_sort_column_id(gElems->column, COL_NAME);

    gElems->renderer = gtk_cell_renderer_text_new ();
    gElems->column = gtk_tree_view_column_new_with_attributes (gStrings->strings[10].multistring[curLan],
                                                               gElems->renderer,
                                                               "text", COL_CREDITS,
                                                               NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (gElems->view), gElems->column);
    gtk_tree_view_column_set_sort_column_id(gElems->column, COL_CREDITS);

    gElems->renderer = gtk_cell_renderer_text_new ();
    gElems->column = gtk_tree_view_column_new_with_attributes (gStrings->strings[11].multistring[curLan],
                                                               gElems->renderer,
                                                               "text", COL_MARK,
                                                               NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (gElems->view), gElems->column);
    gtk_tree_view_column_set_sort_column_id(gElems->column, COL_MARK);

    gElems->renderer = gtk_cell_renderer_text_new ();
    gElems->column = gtk_tree_view_column_new_with_attributes (gStrings->strings[12].multistring[curLan],
                                                               gElems->renderer,
                                                               "text", COL_DATE,
                                                               NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (gElems->view), gElems->column);
    gtk_tree_view_column_set_sort_column_id(gElems->column, COL_DATE);

    gtk_container_add (GTK_CONTAINER (gElems->scrollWindow), gElems->view);

    // Create horizontal box to contain buttons
    gElems->hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_container_add (GTK_CONTAINER (gElems->vbox), gElems->hbox);

    gElems->addButton = gtk_button_new_with_label (gStrings->strings[5].multistring[curLan]);
    gtk_box_pack_start (GTK_BOX (gElems->hbox), gElems->addButton, TRUE, FALSE, 3);

    gElems->removeButton = gtk_button_new_with_label (gStrings->strings[13].multistring[curLan]);
    gtk_box_pack_start (GTK_BOX (gElems->hbox), gElems->removeButton, TRUE, FALSE, 3);

    g_signal_connect (gElems->removeButton, "clicked", G_CALLBACK (removeHandler), (gpointer) &gParam);
    g_signal_connect (gElems->addButton, "clicked", G_CALLBACK (showPopup), (gpointer) &gParam);

    // Add labels for result
    gElems->resultLabel = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (gElems->vbox), gElems->resultLabel, FALSE, FALSE, 0);

    gElems->gradLabel = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (gElems->vbox), gElems->gradLabel, FALSE, FALSE, 10);

    calcAvg(gElems, gStrings, curLan);

    // Connect handler for window resize to store new values in settings
    g_signal_connect(gElems->window, "check-resize", G_CALLBACK (updateWinSize), (gpointer) &gParam);

    // Show window and start GUI cycle
    gtk_widget_show_all (gElems->window);
    gtk_main ();

    return ;
}

void createMenu(GuiParam* data){

    GuiParam* gParam = (GuiParam*) data;
    GuiElems* gElems = gParam->gElems;
    GuiStrings* gStrings = gParam->gStrings;
    int curLan = gParam->curLan;

    gElems->menuBar = gtk_menu_bar_new ();
    gElems->fileMenu = gtk_menu_new ();
    gElems->editMenu = gtk_menu_new ();
    gElems->helpMenu = gtk_menu_new ();
    gElems->chooseLangMenu = gtk_menu_new ();

    gElems->fileMenuLabel = gtk_menu_item_new_with_label ("File");
    gElems->editMenuLabel = gtk_menu_item_new_with_label (gStrings->strings[0].multistring[curLan]);
    gElems->helpMenuLabel = gtk_menu_item_new_with_label (gStrings->strings[1].multistring[curLan]);
    gElems->exitMenuLabel = gtk_menu_item_new_with_label (gStrings->strings[2].multistring[curLan]);
    gElems->infoMenuLabel = gtk_menu_item_new_with_label (gStrings->strings[3].multistring[curLan]);
    gElems->langMenuLabel = gtk_menu_item_new_with_label (gStrings->strings[4].multistring[curLan]);
    gElems->engMenuLabel = gtk_check_menu_item_new_with_label (gStrings->strings[14].multistring[curLan]);
    gElems->itaMenuLabel = gtk_check_menu_item_new_with_label (gStrings->strings[15].multistring[curLan]);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (gElems->fileMenuLabel), gElems->fileMenu); // insert submenu for file
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->fileMenu), gElems->exitMenuLabel); // insert exit entry for file submenu
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->menuBar), gElems->fileMenuLabel); // inserts file submenu in menubar
    g_signal_connect(G_OBJECT(gElems->exitMenuLabel), "activate", G_CALLBACK(gtk_main_quit), NULL);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (gElems->editMenuLabel), gElems->editMenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->editMenu), gElems->langMenuLabel);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->menuBar), gElems->editMenuLabel);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (gElems->langMenuLabel), gElems->chooseLangMenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->chooseLangMenu), gElems->engMenuLabel);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->chooseLangMenu), gElems->itaMenuLabel);

    if(curLan == ENG)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gElems->engMenuLabel), TRUE);
    else if(curLan == ITA)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gElems->itaMenuLabel), TRUE);

    g_signal_connect(G_OBJECT(gElems->engMenuLabel), "activate", G_CALLBACK(changeLanguage), (gpointer) gParam);
    g_signal_connect(G_OBJECT(gElems->itaMenuLabel), "activate", G_CALLBACK(changeLanguage), (gpointer) gParam);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (gElems->helpMenuLabel), gElems->helpMenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->helpMenu), gElems->infoMenuLabel);
    gtk_menu_shell_append (GTK_MENU_SHELL (gElems->menuBar), gElems->helpMenuLabel);
    g_signal_connect(G_OBJECT(gElems->infoMenuLabel), "activate", G_CALLBACK(showAboutPopup), (gpointer) gParam);

    gtk_box_pack_start (GTK_BOX (gElems->vbox), gElems->menuBar, FALSE, FALSE, 3);

    return ;
}

void showPopup (GtkButton *addButton, gpointer data){

    GtkWidget *dialog;
    GtkWidget *contentArea;
    GtkWidget *label;
    GtkWidget *entryName;
    GtkWidget *spinButtonCredits;
    GtkWidget *hbox;
    GtkWidget *spinButtonMark;
    GtkWidget *hSeparator;
    GtkWidget *calendar;
    GtkWidget *grid;
    GuiParam *gParam = (GuiParam*) data;
    GuiElems *gElems = gParam->gElems;
    FileNames *fNames = gParam->fNames;
    GuiStrings *gStrings = gParam->gStrings;

    MarkRow *mRow;
    GtkTreeIter iter;

    FILE *fp = NULL;

    int curLan = gParam->curLan;
    int result;
    unsigned int tmpDay, tmpMonth, tmpYear;

    dialog = gtk_dialog_new_with_buttons (gStrings->strings[7].multistring[curLan],
                                          GTK_WINDOW(gElems->window),
                                          GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                          gStrings->strings[8].multistring[curLan],
                                          GTK_RESPONSE_REJECT,
                                          "OK",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    contentArea = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    grid = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    label = gtk_label_new (gStrings->strings[9].multistring[curLan]);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    entryName = gtk_entry_new ();
    gtk_grid_attach(GTK_GRID(grid), entryName, 1, 0, 1, 1);

    label = gtk_label_new (gStrings->strings[10].multistring[curLan]);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    spinButtonCredits = gtk_spin_button_new_with_range (1, 30, 1);
    gtk_grid_attach(GTK_GRID(grid), spinButtonCredits, 1, 1, 1, 1);

    label = gtk_label_new (gStrings->strings[11].multistring[curLan]);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    spinButtonMark = gtk_spin_button_new_with_range (18, 30, 1);
    gtk_grid_attach(GTK_GRID(grid), spinButtonMark, 1, 2, 1, 1);

    label = gtk_label_new (gStrings->strings[12].multistring[curLan]);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);

    calendar = gtk_calendar_new ();
    gtk_grid_attach(GTK_GRID(grid), calendar, 1, 3, 1, 1);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX (contentArea), hbox, TRUE, FALSE, 3);

    gtk_box_pack_start (GTK_BOX (hbox), grid, TRUE, FALSE, 3);

    hSeparator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start (GTK_BOX (contentArea), hSeparator, TRUE, FALSE, 3);

    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

    gtk_widget_show_all (dialog);

    result = gtk_dialog_run (GTK_DIALOG (dialog));

    switch (result){
        case GTK_RESPONSE_ACCEPT:

            /* Add new row */

            gtk_calendar_get_date(GTK_CALENDAR(calendar), &tmpYear, &tmpMonth, &tmpDay);

            mRow = createMarkRow(gElems->newRowNumber,
                                 gtk_entry_get_text(GTK_ENTRY(entryName)),
                                 gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinButtonCredits)),
                                 gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinButtonMark)),
                                 tmpDay, tmpMonth, tmpYear);

            if(strlen(mRow->name) > 0 && mRow->credits > 0){

                gtk_list_store_append (gElems->store, &iter);  /* Acquire an iterator */

                gtk_list_store_set (gElems->store, &iter,
                                    COL_NUM, mRow->num,
                                    COL_NAME, mRow->name,
                                    COL_CREDITS, mRow->credits,
                                    COL_MARK, mRow->mark,
                                    COL_DATE, mRow->date,
                                    -1);
                calcAvg(gElems, gStrings, curLan);

                /* Update data file */
                fp = fopen(fNames->dataFileName, "a"); // should "always" succeed

                if(gElems->numElems > 0)
                    fprintf(fp, "\n");
                fprintf(fp, "%d,%s,%d,%d,%s", mRow->num, mRow->name, mRow->credits, mRow->mark, mRow->date);

                fclose(fp);

                gElems->numElems++;
                gElems->newRowNumber++;
            }

            freeMarkRow(mRow);
            break;
        default:

            break;
    }

    gtk_widget_destroy (dialog);

    return ;
}

void removeHandler(GtkButton *removeButton, gpointer data){

    FILE *fpIn = NULL;
    FILE *fpOut = NULL;
    int rowNumber;
    int count = 0;
    char* token;
    char buff[BUFSIZE], buffOut[BUFSIZE];

    GuiParam* gParam = (GuiParam*) data;
    GuiElems* gElems = gParam->gElems;
    GuiStrings *gStrings = gParam->gStrings;
    FileNames* fNames = gParam->fNames;
    int curLan = gParam->curLan;

    rowNumber = deleteElemFromListStore(gElems);

    gElems->numElems--;

    calcAvg(gElems, gStrings, curLan);

    fpIn = fopen(fNames->dataFileName, "r");
    fpOut = fopen(fNames->tmpFileName, "w");

    while(fgets(buff, BUFSIZE, fpIn) != NULL){
        strcpy(buffOut, buff);
        token = strtok(buff, ",");
        if(atoi(token) != rowNumber){
            if(buffOut[strlen(buffOut) - 1] == '\n')
                buffOut[strlen(buffOut) - 1] = '\0';
            if(count > 0)
                fprintf(fpOut, "\n");
            fprintf(fpOut, "%s", buffOut);
            count++;
        }
    }

    fclose(fpOut);
    fclose(fpIn);

    unlink(fNames->dataFileName);
    rename(fNames->tmpFileName, fNames->dataFileName);

    return ;
}

void updateWinSize(GtkWidget *window, gpointer data){

    GuiParam* gParam = (GuiParam*) data;
    GuiElems* gElems = gParam->gElems;
    Settings* settings = gParam->settings;
    int newWidth, newHeight;

    gtk_window_get_size((GtkWindow*)gElems->window, &newWidth, &newHeight);

    settings->width = newWidth;
    settings->height = newHeight;

    return ;
}

void changeLanguage(GtkMenuItem *langMenuLabel, gpointer data){

    GtkWidget* dialog;
    GuiParam* gParam = (GuiParam*) data;
    GuiStrings* gStrings = gParam->gStrings;
    GuiElems* gElems = gParam->gElems;
    Settings* settings = gParam->settings;
    int curLan = gParam->curLan;
    int newLan = 0;
    const char* label = gtk_menu_item_get_label (langMenuLabel);

    // Change language only if the selected one is different than current one
    if(curLan != ENG && strcmp(label, gStrings->strings[14].multistring[curLan]) == 0){
        settings->language = ENG;
        newLan = 1;
    }
    else if(curLan != ITA && strcmp(label, gStrings->strings[15].multistring[curLan]) == 0){
        settings->language = ITA;
        newLan = 1;
    }

    if(newLan == 1){
        dialog = gtk_message_dialog_new (GTK_WINDOW(gElems->window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_CLOSE,
                                         "%s", gStrings->strings[16].multistring[curLan]);
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
    }

    return ;
}

void showAboutPopup(GtkMenuItem *infoMenuLabel, gpointer data){

    GuiParam* gParam = (GuiParam*) data;
    GuiStrings* gStrings = gParam->gStrings;
    GuiElems* gElems = gParam->gElems;
    int curLan = gParam->curLan;

    GtkWidget* dialog = gtk_about_dialog_new ();

    gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(dialog), "Libretron");
    gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(dialog), "http://www.pyrox.it");
    gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG(dialog), gElems->icon);
    gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG(dialog), "Loris \"Pyrox\" Gabriele");
    gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG(dialog), gStrings->strings[18].multistring[curLan]);

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

    return ;
}

void calcAvg(GuiElems* gElems, GuiStrings* gStrings, int curLan){

    int credits, mark, cnt, gradMark;
    double weightAvg;
    char buff[BUFSIZE];
    GtkTreeIter iter;

    cnt = 0;
    weightAvg = 0;
    gradMark = 0;

    if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gElems->store), &iter)){
        gtk_tree_model_get(GTK_TREE_MODEL(gElems->store), &iter, COL_CREDITS, &credits, COL_MARK, &mark, -1);
        weightAvg += (credits * mark);
        cnt += credits;

        while(gtk_tree_model_iter_next(GTK_TREE_MODEL(gElems->store), &iter)){
            gtk_tree_model_get(GTK_TREE_MODEL(gElems->store), &iter, COL_CREDITS, &credits, COL_MARK, &mark, -1);
            weightAvg += (credits * mark);
            cnt += credits;
        }

        weightAvg /= cnt;
    }

    sprintf(buff, "%s: %.1f", gStrings->strings[17].multistring[curLan], weightAvg);
    gtk_label_set_text(GTK_LABEL(gElems->resultLabel), buff);

    gradMark = weightAvg * 110 / 30;

    sprintf(buff, "%s: %d", gStrings->strings[19].multistring[curLan], gradMark);
    gtk_label_set_text(GTK_LABEL(gElems->gradLabel), buff);

    return ;
}
