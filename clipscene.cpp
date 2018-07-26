#include "clipscene.h"
#include "select.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QScrollBar>

ClipScene::ClipScene(QObject* parent, QGraphicsView* view) : QGraphicsScene(parent)
{
    this->view = view;
}

void ClipScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //remove old selection
    if(selectionRect){
        delete(selectionRect);
        selectionRect = NULL;
        if(m_selection) delete m_selection;

    }
    if (getSelectionMode()){
        //creat new selection
        if (event->button() & Qt::LeftButton)
        {
            m_leftMouseButtonPressed = true;
            setPreviousPosition(event->scenePos());
            m_selection = new QGraphicsRectItem();
            m_selection->setBrush(QBrush(QColor(158,182,255,96)));
            m_selection->setPen(QPen(QColor(158,182,255,200),1));
            addItem(m_selection);
        }
    }
    else {
        setPreviousPosition(event->scenePos());
        //scrollRect = sceneRect();
    }

    QGraphicsScene::mousePressEvent(event);
}

int max(double x, int y){
    if(x > y)   return x;
    return y;
}

void ClipScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (getSelectionMode()){
        // modify the selection as the mouse moves
        if (m_leftMouseButtonPressed)
        {
            auto dx = event->scenePos().x() - m_previousPosition.x();
            auto dy = event->scenePos().y() - m_previousPosition.y();
            auto resultDelta = qMax(qAbs(dx), qAbs(dy));
            m_selection->setRect((dx > 0) ? m_previousPosition.x() : m_previousPosition.x() - qAbs(dx),
                               (dy > 0) ? m_previousPosition.y() : m_previousPosition.y() - qAbs(dy),
                               qAbs(dx),
                               qAbs(dy));
        }
    }
    else {
        //auto dx = m_previousPosition.x() - event->scenePos().x();
        //auto dy = m_previousPosition.y() - event->scenePos().y();
        //view->centerOn(event->scenePos().x(), event->scenePos().y());
        //setSceneRect(scrollRect);
        //setPreviousPosition(event->scenePos());
       // QPointF oldp = view->mapToScene(m_previousPosition.x(), m_previousPosition.y());
       // QPointF newp = view->mapToScene(event->pos().x(), event->pos().y());
       // QPointF translation = newp - oldp;

        //  view->translate(translation.x(), translation.y());
        //qDebug("HOZ %lf - %lf - %d\n",event->scenePos().x(),m_previousPosition.x(),view->horizontalScrollBar()->value());
        //qDebug("VERT %lf - %lf - %d\n",event->scenePos().y(),m_previousPosition.y(),view->verticalScrollBar()->value());

        //view->horizontalScrollBar()->setValue(max(view->horizontalScrollBar()->value() - (event->scenePos().x() - m_previousPosition.x()),0));
        //view->verticalScrollBar()->setValue(max(view->verticalScrollBar()->value() - (event->scenePos().y() - m_previousPosition.y()),0));

        //qDebug("HOZ %lf - %lf - %lf\n",event->scenePos().x(),m_previousPosition.x(),view->horizontalScrollBar()->value());
        //qInfo("Horizintal" + to_string(event->scenePos().x()) + " " + m_previousPosition.x() + " " + view->horizontalScrollBar()->value());
        //qInfo() << "Horizintal" << event->scenePos().x() << " " << m_previousPosition.x() << " " << view->horizontalScrollBar()->value();
        //qInfo() << "Vertical" << event->scenePos().y() << " " << m_previousPosition.y() << " " << view->verticalScrollBar()->value();
        setPreviousPosition(event->scenePos());
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void ClipScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (getSelectionMode()){
        setSelectionMode(false);
        if (event->button() & Qt::LeftButton)
        {
            m_leftMouseButtonPressed = false;

            //check if the selected area > 25
            if(m_selection->rect().height() > 5){
                selectionRect = new QRect(m_selection->boundingRect().toRect());
            }

        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ClipScene::applyCrop()
{
    //if there is image and selected area
    if(m_currentImageItem && selectionRect){

        //change image to new area
        changeImage(m_currentImageItem->pixmap().copy(*selectionRect));

        //delete the selected area
        delete selectionRect;
        selectionRect = NULL;
    }

    if(m_selection){
        delete m_selection;
        m_selection = NULL;
    }
}

void ClipScene::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(m_previousPosition);
}

QPointF ClipScene::previousPosition() const
{
    return m_previousPosition;
}

void ClipScene::setImage(const QString& filePath)
{
    //remove old image
    if (m_currentImageItem){
        this->removeItem(m_currentImageItem);
    }

    //set the original image && the curr ones.
    original = new QPixmap(filePath);
    currImg = original;
    currImgUnRotated = original;

    //create Pixmap item and add it to the scene
    m_currentImageItem = new QGraphicsPixmapItem(QPixmap(filePath));
    addItem(m_currentImageItem);
}

void ClipScene::changeImage(const QPixmap& pixmap){

    //remove old image
    if (m_currentImageItem){
        this->removeItem(m_currentImageItem);
    }

    //delete old currImg
    if(currImg != original && currImg != currImgUnRotated)
        delete(currImg);

    //set the curr ones.
    currImg = new QPixmap(pixmap);
    currImgUnRotated = currImg;

    //delete the old object
    delete(m_currentImageItem);

    //create Pixmap item and add it to the scene
    m_currentImageItem = new QGraphicsPixmapItem(pixmap);
    addItem(m_currentImageItem);
}

void ClipScene::changeImageRotate(const QPixmap& pixmap){

    //remove old image
    if (m_currentImageItem){
        this->removeItem(m_currentImageItem);
    }

    //delete old currImg
    if(currImg != original && currImg != currImgUnRotated)
        delete(currImg);

    //set the curr one.
    currImg = new QPixmap(pixmap);

    //delete the old object
    delete(m_currentImageItem);

    //create Pixmap item and add it to the scene
    m_currentImageItem = new QGraphicsPixmapItem(pixmap);
    addItem(m_currentImageItem);
}


void ClipScene::resetImage(){

    //if there is original image, then use it to change the curr one.
    if(original){
        this->changeImage(*original);
    }

}

QPixmap* ClipScene::getCurr(){
    return currImg;
}

QPixmap* ClipScene::getCurrImgUnRotated(){
    return currImgUnRotated;
}

QRect* ClipScene::getSelectionRect(){
    return selectionRect;
}
