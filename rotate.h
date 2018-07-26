#ifndef ROTATE_H
#define ROTATE_H

#include <QMainWindow>
#include <QGraphicsView>
#include "clipscene.h"


class rotate : public QObject
{
    Q_OBJECT

public:

    rotate()=delete;

    rotate(ClipScene* view);

    void execute();

    //save the last rotate value
    int deg;

public slots:

    void rotateSlot(int deg);

private:

    //the scene containing the image
    ClipScene *view;

    int last;

    //function to rotate given image wih given degree
    QImage inRotate(QImage& img, int val);
};

#endif // ROTATE_H
