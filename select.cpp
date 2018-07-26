#include <QMainWindow>
#include "select.h"
#include "clipscene.h"

bool selectionMode;

void setSelectionMode(bool mode){
    selectionMode = mode;
}

bool getSelectionMode(){
    return selectionMode;
}

void selectClicked(){
    selectionMode = !selectionMode;
}

