#ifndef CLIPSCENE_H
#define CLIPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>

class QGraphicsSceneMouseEvent;
class QGraphicsPixmapItem;
class QGraphicsRectItem;

class ClipScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:

    ClipScene(QObject* parent, QGraphicsView* view);
    QPointF previousPosition() const;

    //set image for the first time from File browser
    void setImage(const QString& filePath);

    //apply the crop if there is selection
    void applyCrop();

    //change the image in the scene to a given one, usualy used to crop the image
    void changeImage(const QPixmap& pixmap);

    //change the image to given one, used only if the given image is rotated
    void changeImageRotate(const QPixmap& pixmap);

    //reset the image to the first loaded one
    void resetImage();

    //get some private data to save and rotate
    QPixmap* getCurr();
    QPixmap* getCurrImgUnRotated();
    QRect* getSelectionRect();

signals:

    void previousPositionChanged(const QPointF previousPosition);
    void clippedImage(const QPixmap& pixmap);

public slots:

    void setPreviousPosition(const QPointF previousPosition);


protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;


private:

    //rect for selection
    QGraphicsRectItem* m_selection          {nullptr};
    QGraphicsPixmapItem* m_currentImageItem {nullptr};
    QPointF m_previousPosition;
    bool m_leftMouseButtonPressed           {false};
    QRect* selectionRect                    {nullptr};

    //QRectF scrollRect;
    const double SCROLL_EPS = 25.0;

    //curr Image && curr un-rotated Image && Original Image
    QPixmap* currImg                        {nullptr};
    QPixmap* currImgUnRotated               {nullptr};
    QPixmap* original                       {nullptr};
    QGraphicsView* view;
};

#endif
