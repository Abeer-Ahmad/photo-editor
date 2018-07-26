#include <QMainWindow>
#include "zoom.h"

zoom::zoom()
{
    factor = 1.5;
    currentScale = 1.0;  // stores the current scale value.

}

void zoom::zoomIn(QGraphicsView* obj){
    if(currentScale < 18){
        obj->scale(factor,factor);
        currentScale *= factor;
    }
}

void zoom::zoomInSelection(QGraphicsView* obj, QRect* selectionRect){
    if(!selectionRect){
        zoomIn(obj);
        return;
    }
    if(currentScale < 18){
     obj->centerOn(selectionRect->center());
     obj->scale(factor,factor);
     currentScale *= factor;
   }
}

void zoom::zoomOut(QGraphicsView* obj){
    if(currentScale > 0.2){
        obj->scale(1/factor,1/factor);
        currentScale /= factor;
    }
}
