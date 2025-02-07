#include "scene.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "robot.h"
#include "obstacle.h"
#include <QTimer>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent},
      stop(false),
      id(0)
{
    mTimer = new QTimer(this);
    timer = new QTimer(this);
    timerLeft = new QTimer(this);
    timerRight = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Scene::timeout);


    endOfScene.append(new QGraphicsLineItem(0, 0, 0, 690));
    endOfScene.append(new QGraphicsLineItem(0, 0, 990, 0));
    endOfScene.append(new QGraphicsLineItem(0, 690, 990, 690));
    endOfScene.append(new QGraphicsLineItem(990, 0, 990, 690));
}

QString Scene::toJSON()
{
    QJsonArray arr;
    for (auto *obstacle : mObstacles)
    {
        // rotation
        qreal rotationAngle = obstacle->rotationAngle();

        // scale
        qreal scaleFactor = obstacle->scaleFactor();

        // type
        QString type = "Obstacle";

        arr.append(QJsonObject({
            {"x", obstacle->x()},
            {"y", obstacle->y()},
            {"object", type},
            {"rotation", rotationAngle},
            {"scale", scaleFactor}
        }));
    }
    for (auto *robot : mRobots)
    {
        // rotation
        qreal rotationAngle = robot->rotationAngle();

        // scale
        qreal scaleFactor = 1.0;

        // type
        QString type = "Robot";

        arr.append(QJsonObject({
            {"x", robot->x()},
            {"y", robot->y()},
            {"object", type},
            {"rotation", rotationAngle},
            {"scale", scaleFactor}
        }));
    }
    for (auto *player : mPlayers)
    {
        // rotation
        qreal rotationAngle = player->rotationAngle();

        // scale
        qreal scaleFactor = 1.0;

        // type
        QString type = "Player";

        arr.append(QJsonObject({
            {"x", player->x()},
            {"y", player->y()},
            {"object", type},
            {"rotation", rotationAngle},
            {"scale", scaleFactor}
        }));
    }

    QJsonObject obj{{"items", arr}};
    QJsonDocument doc;
    doc.setObject(obj);

    return doc.toJson(QJsonDocument::Indented);
}

void Scene::start()
{
    for (Robot *robot : mRobots)
    {
        robot->setMovementEnable(false);
    }
    for (Robot *player : mPlayers)
    {
        emit isPlayer();
        player->setMovementEnable(false);
    }
    for (Obstacle *obstacle : mObstacles)
    {
        obstacle->setMovementEnable(false);
    }
    mTimer->start(TIME_TO_REFRESH_TIMER);
    timer->start(TIME_TO_REFRESH_TIMER);
}

void Scene::pause()
{
    for (Robot *robot : mRobots)
    {
        robot->setMovementEnable(true);
    }
    for (Robot *player : mPlayers)
    {
        emit isPlayer();
        player->setMovementEnable(true);
    }
    for (Obstacle *obstacle : mObstacles)
    {
        obstacle->setMovementEnable(true);
    }
    mTimer->stop();
    timer->stop();
}

void Scene::timeout()
{
    for (Robot *robot : mRobots)
    {
        if (!obstacleInWay(robot))
        {
            robot->moveForward();
        }
        else
        {
            // try to change direction
            robot->rotate(-1.0); //! DEBUG
        }
    }
}

void Scene::forward()
{
    stop = false;
    for (Robot *player : mPlayers)
    {
        disconnect(timer, &QTimer::timeout, nullptr, nullptr);
        connect(timer, &QTimer::timeout, [=]()
        {
            if (!obstacleInWay(player) && !stop)
            {
                player->moveForward();
            }
            else
            {
                timer->stop();
            }
        });
        timer->start(TIME_TO_REFRESH_TIMER);
    }
}

void Scene::stopPlayer()
{
    stop = true;
}

void Scene::left()
{
    for (Robot *player : mPlayers)
    {
        disconnect(timerLeft, &QTimer::timeout, nullptr, nullptr);
        connect(timerLeft, &QTimer::timeout, [=]()
        {
            player->rotate(-1.0);
        });
        timerLeft->start(TIME_TO_REFRESH_TIMER);
    }
}

void Scene::right()
{
    for (Robot *player : mPlayers)
    {
        disconnect(timerRight, &QTimer::timeout, nullptr, nullptr);
        connect(timerRight, &QTimer::timeout, [=]()
        {
            player->rotate(1.0);
        });
        timerRight->start(TIME_TO_REFRESH_TIMER);
    }
}

void Scene::leftEnd()
{
    timerLeft->stop();
}

void Scene::rightEnd()
{
    timerRight->stop();
}

void Scene::spawnObstacle()
{
    Obstacle *obstacle = new Obstacle;

    addItem(obstacle);
    mObstacles.append(obstacle);
}

void Scene::spawnObstacleFromLoad(qreal x, qreal y, qreal rotation, qreal scale)
{
    Obstacle *obstacle = new Obstacle;
    obstacle->changePosition(x, y);
    obstacle->changeRotationAngle(rotation);
    obstacle->changeScaleFactor(scale);
    addItem(obstacle);
    mObstacles.append(obstacle);
}

void Scene::spawnRobot()
{
    Robot *robot = new Robot(id++);

    addItem(robot);
    mRobots.append(robot);
    emit addRobotToList();
}

bool playerRobot = false;

void Scene::spawnRobotFromLoad(qreal x, qreal y, qreal rotation, bool player)
{
    if (player)
    {
        Robot *player = new Robot(-1);
        player->changePosition(x, y);
        player->changeRotationAngle(rotation);
        addItem(player);
        mPlayers.append(player);
        playerRobot = true;
    }
    else
    {
        Robot *robot = new Robot(id++);
        robot->changePosition(x, y);
        robot->changeRotationAngle(rotation);
        addItem(robot);
        mRobots.append(robot);
        emit addRobotToList();
    }
}

void Scene::clearArraysAfterLoad()
{
    mPlayers.clear();
    mRobots.clear();
    mObstacles.clear();
}

void Scene::spawnRobotPlayer()
{
    if (!playerRobot)
    {
        Robot *player = new Robot(-1);
        addItem(player);
        mPlayers.append(player);
        playerRobot = true;
    }
}

bool Scene::obstacleInWay(Robot *robot)
{
    for (Obstacle *obstacle : mObstacles)
    {
        if (robot->collidesWithItem(obstacle))
        {
            return true;
        }
    }
    for (QGraphicsLineItem *lineAtTheEnd : endOfScene)
    {
        if (robot->collidesWithItem(lineAtTheEnd))
        {
            return true;
        }
    }
//    for (Robot *otherRobot : mRobots) //TODO
//    {
//        if (robot == otherRobot)
//        {
//            continue;
//        }
//        if (robot->collidesWithPath(otherRobot->detectionShape()))
//        {
//            return true;
//        }
//    }
    return false;
}

Robot *Scene::getRobot(int robotID)
{
    for (Robot *robot : mRobots)
    {
        if (robotID == robot->getRobotID())
        {
            return robot;
        }
    }
    return NULL;
}
