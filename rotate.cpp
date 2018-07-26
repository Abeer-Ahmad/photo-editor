#include "rotate.h"
#include "clipscene.h"
#include <QSpinBox>
#include <QSlider>
#include <QObject>
#include <QDialog>
#include <QPoint>
#include <QHBoxLayout>

rotate::rotate(ClipScene *view)
{
    deg = 0;
    this->view = view;
    last = 0;
}

void rotate::execute(){

    //create spinner and slider
    QSlider* slider = new QSlider(Qt::Horizontal);

    //set ranges for both
    slider->setRange(-180,180);

    //connect spinner with slider and rotateSlot
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(rotateSlot(int)));

    //set default value
    slider->setValue(deg);

    //create new dialog and set title
    QDialog* di = new QDialog();
    di->setWindowTitle("Rotate");

    //create layout and add spinner and slider
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(slider);

    //add the layout to the dialog and show them to the user
    di->setLayout(layout);
    di->show();
}


QImage rotate::inRotate(QImage& img, int val){

    //get center of img
    QPoint center = img.rect().center();

    //create matrix of the image and rotate it
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(val);

    //traansform and return the new img
    QImage dstImg = img.transformed(matrix);
    return dstImg;

}
void rotate::rotateSlot(int val){

    //if no image return
    if(!view->getCurrImgUnRotated())    return;

    deg = deg+val;

    //get the last unrotated img
    QPixmap* srcImg = view->getCurrImgUnRotated();

    //get center of img
    QPoint center = srcImg->rect().center();

    //create matrix of the image and rotate it
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(deg);

    //rotate it
    QPixmap dstPix = srcImg->transformed(matrix);

    //change the image in the view and save the new rotate value
    view->changeImageRotate(dstPix);
}
