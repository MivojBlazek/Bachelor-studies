#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsItemGroup>
#include <QGraphicsItem>

class Robot : public QGraphicsItemGroup
{
    friend class Scene;

public:
    Robot(int id, QGraphicsItem *parent = nullptr);

    int getRobotID() const;
    qreal rotationAngle() const;
    void changeRotationAngle(qreal rotation);
    void setMovementEnable(bool enable);
    void changePosition(qreal x, qreal y);
    void hideRect(bool hide);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QPainterPath detectionShape() const;

public slots:
    void moveForward();

private:
    qreal mRotationAngle;
    int robotID;

    QGraphicsLineItem *mLine;
    QGraphicsEllipseItem *mEllipse;
    QGraphicsRectItem *mRect;
    void rotate(qreal angle);
};

#endif // ROBOT_H
