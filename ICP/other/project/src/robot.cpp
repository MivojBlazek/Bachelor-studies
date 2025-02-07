#include "robot.h"
#include "scene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include <QGraphicsItemGroup>
#include <QThread>

Robot::Robot(int id, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    mRotationAngle(0.0),
    robotID(id)
{
    mLine = new QGraphicsLineItem(200, 240, 220, 240, this);
    mEllipse = new QGraphicsEllipseItem(180, 220, 40, 40, this);
    mRect = new QGraphicsRectItem(200, 220, 80, 40, this);

    if (id < 0)
    {
        QPen playerPen(Qt::blue);
        mLine->setPen(playerPen);
        mEllipse->setPen(playerPen);
        mRect->setPen(playerPen);
    }

    addToGroup(mLine);
    addToGroup(mEllipse);
    addToGroup(mRect);

    setFlag(QGraphicsItem::ItemIsMovable);

    setTransformOriginPoint(QPointF(200,240));
}

int Robot::getRobotID() const
{
    return robotID;
}

void Robot::hideRect(bool hide)
{
    Q_UNUSED(hide);
    //mRect->setVisible(!hide);
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // rotation menu
        QMenu menu;

        menu.addSection("RobotID: " + QString::number(robotID));

        QAction *rotateLeftAction = menu.addAction("Rotate left");
        QAction *rotateRightAction = menu.addAction("Rotate right");

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == rotateLeftAction)
        {
            rotate(-45.0);
        }
        else if (selectedAction == rotateRightAction)
        {
            rotate(45.0);
        }
    }
    QGraphicsItemGroup::mousePressEvent(event);
}

void Robot::rotate(qreal angle)
{
    mRotationAngle += angle;
    setRotation(mRotationAngle);
}

qreal Robot::rotationAngle() const
{
    return mRotationAngle;
}

void Robot::changeRotationAngle(qreal rotation)
{
    mRotationAngle = rotation;
    rotate(0.0);
}

void Robot::setMovementEnable(bool enable)
{
    setFlag(QGraphicsItem::ItemIsMovable, enable);
}

void Robot::changePosition(qreal x, qreal y)
{
    setPos(pos() + QPointF(x, y));
}

void Robot::moveForward()
{
    qreal angleInRad = qDegreesToRadians(mRotationAngle);
    QPointF direction(qCos(angleInRad), qSin(angleInRad));

    QPointF newPosition = pos() + direction;

    setPos(newPosition);
}

QPainterPath Robot::detectionShape() const
{
    QPainterPath path;
    path.addRect(mEllipse->boundingRect());
    return path;
}
