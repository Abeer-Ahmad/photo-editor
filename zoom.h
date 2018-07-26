#ifndef ZOOM_H
#define ZOOM_H

#include <QGraphicsView>

class zoom : public QObject
{
public:

    zoom();

    void zoomIn(QGraphicsView* obj);

    void zoomOut(QGraphicsView* obj);

    void zoomInSelection(QGraphicsView* obj, QRect* selectionRect);
private:

    double factor;

    double currentScale;

};

#endif // ZOOM_H
