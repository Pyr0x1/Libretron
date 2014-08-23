#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modelmanager.h"

int initListStore(GuiElems* gElems, FileNames* fNames){

    FILE *fp = NULL;
    MarkRow* mRow = NULL;
    GtkTreeIter iter;
    char buff[BUFSIZE];

    gElems->newRowNumber = 0;
    gElems->numElems = 0;

    gElems->store = gtk_list_store_new (COL_TOT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);

    if((fp = fopen(fNames->dataFileName, "r")) == NULL){
        printf("Error while opening data file (maybe there isn't one yet)\n");
        return gElems->numElems; // 0
    }

    while(fgets(buff, BUFSIZE, fp) != NULL){

        mRow = tokenizer(buff, ",");

        gtk_list_store_append (gElems->store, &iter);  /* Acquire an iterator */

        gtk_list_store_set (gElems->store, &iter,
                            COL_NUM, mRow->num,
                            COL_NAME, mRow->name,
                            COL_CREDITS, mRow->credits,
                            COL_MARK, mRow->mark,
                            COL_DATE, mRow->date,
                            -1);

        gElems->newRowNumber = mRow->num + 1;
        gElems->numElems++;
        freeMarkRow(mRow);
    }

    fclose(fp);

    return gElems->numElems;
}

int deleteElemFromListStore(GuiElems* gElems){

    GtkTreeSelection *treeSelection;
    GList *pathList;
    GtkTreePath *path;
    GtkTreeIter iter;
    int rowNumber;

    treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gElems->view));
    pathList = gtk_tree_selection_get_selected_rows(treeSelection, NULL);
    path = (GtkTreePath*) g_list_nth_data(pathList, 0);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(gElems->store), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(gElems->store), &iter, COL_NUM, &rowNumber, -1);
    gtk_list_store_remove(gElems->store, &iter);

    g_list_free_full (pathList, (GDestroyNotify) gtk_tree_path_free);

    return rowNumber;
}

void compactDataFile(FileNames* fNames){

    FILE *fpIn = NULL;
    FILE *fpOut = NULL;
    MarkRow* tmpRow = NULL;
    char buff[BUFSIZE];
    int cnt = 0;

    fpIn = fopen(fNames->dataFileName, "r");
    fpOut = fopen(fNames->tmpFileName, "w");

    while(fgets(buff, BUFSIZE, fpIn) != NULL){
        tmpRow = tokenizer(buff, ",");

        if(cnt > 0)
            fprintf(fpOut, "\n");
        fprintf(fpOut, "%d,%s,%d,%d,%s", cnt, tmpRow->name, tmpRow->credits, tmpRow->mark, tmpRow->date);

        cnt++;
    }

    fclose(fpIn);
    fclose(fpOut);

    unlink(fNames->dataFileName);
    rename(fNames->tmpFileName, fNames->dataFileName);

    return ;
}
