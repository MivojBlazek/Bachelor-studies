#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>

class Obstacle : public QGraphicsRectItem
{
public:
    Obstacle(QGraphicsItem *parent = nullptr);

    qreal rotationAngle() const;
    qreal scaleFactor() const;
    void changeRotationAngle(qreal rotation);
    void changeScaleFactor(qreal scaleToChange);
    void changePosition(qreal x, qreal y);

    void setMovementEnable(bool enable);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    qreal mRotationAngle;
    qreal mScaleFactor;
    void rotate(qreal angle);
    void scale(qreal scalingFactor);
};

#endif // OBSTACLE_H
