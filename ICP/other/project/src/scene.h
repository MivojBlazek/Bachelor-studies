#ifndef SCENE_H
#define SCENE_H

#define TIME_TO_REFRESH_TIMER 50

#include "robot.h"
#include "obstacle.h"

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    QString toJSON();

    void start();
    void pause();

    void timeout();

    void forward();
    void stopPlayer();
    void left();
    void right();
    void leftEnd();
    void rightEnd();

    void spawnObstacle();
    void spawnRobot();
    void spawnRobotPlayer();

    void spawnObstacleFromLoad(qreal x, qreal y, qreal rotation, qreal scale);
    void spawnRobotFromLoad(qreal x, qreal y, qreal rotation, bool player = false);

    void clearArraysAfterLoad();

    Robot *getRobot(int robotID);

signals:
    void isPlayer();
    void addRobotToList();

private:
    bool stop;
    int id;

    QTimer *mTimer;
    QTimer *timer;
    QTimer *timerLeft;
    QTimer *timerRight;
    QList<Robot *> mRobots;
    QList<Robot *> mPlayers;
    QList<Obstacle *> mObstacles;

    QList<QGraphicsLineItem *> endOfScene;

    bool obstacleInWay(Robot *player);
};

#endif // SCENE_H
