#include "obstacle.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPen>

Obstacle::Obstacle(QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    mRotationAngle(0.0),
    mScaleFactor(1.0)
{
    setRect(200, 180, 20, 30);

    setFlag(QGraphicsItem::ItemIsMovable);

    setTransformOriginPoint(QPointF(210, 195));
}


void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // rotation menu
        QMenu menu;
        QAction *rotateLeftAction = menu.addAction("Rotate left");
        QAction *rotateRightAction = menu.addAction("Rotate right");
        QAction *scaleUpAction = menu.addAction("Scale up");
        QAction *scaleDownAction = menu.addAction("Scale down");

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == rotateLeftAction)
        {
            rotate(-45.0 / 2.0);
        }
        else if (selectedAction == rotateRightAction)
        {
            rotate(45.0 / 2.0);
        }
        else if (selectedAction == scaleUpAction)
        {
            scale(1.25);
        }
        else if (selectedAction == scaleDownAction)
        {
            scale(0.8);
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void Obstacle::rotate(qreal angle)
{
    mRotationAngle += angle;
    setRotation(mRotationAngle);
}

void Obstacle::scale(qreal scalingFactor)
{
    mScaleFactor *= scalingFactor;
    setScale(mScaleFactor);
}

qreal Obstacle::rotationAngle() const
{
    return mRotationAngle;
}

qreal Obstacle::scaleFactor() const
{
    return mScaleFactor;
}

void Obstacle::changeRotationAngle(qreal rotation)
{
    mRotationAngle = rotation;
    rotate(0.0);
    update();
}

void Obstacle::changeScaleFactor(qreal scaleToChange)
{
    mScaleFactor = scaleToChange;
    scale(1.0);
}

void Obstacle::changePosition(qreal x, qreal y)
{
    setPos(pos() + QPointF(x, y));
}

void Obstacle::setMovementEnable(bool enable)
{
    setFlag(QGraphicsItem::ItemIsMovable, enable);
}
