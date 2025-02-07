#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robotlist.h"
#include "scene.h"
#include <QGraphicsItem>
#include "menu.h"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1600, 900);
    Scene *scene = initScene();
    robotList = initRobotList();

    connect(ui->menu, &Menu::spawnObstacle, scene, &Scene::spawnObstacle);
    connect(ui->menu, &Menu::spawnRobot, scene, &Scene::spawnRobot);
    connect(scene, &Scene::addRobotToList, robotList, &RobotList::spawnRobot);

    connect(ui->menu, &Menu::spawnPlayer, scene, &Scene::spawnRobotPlayer);

    connect(ui->menu, &Menu::forward, scene, &Scene::forward);
    connect(ui->menu, &Menu::stopPlayer, scene, &Scene::stopPlayer);
    connect(ui->menu, &Menu::left, scene, &Scene::left);
    connect(ui->menu, &Menu::right, scene, &Scene::right);
    connect(ui->menu, &Menu::leftEnd, scene, &Scene::leftEnd);
    connect(ui->menu, &Menu::rightEnd, scene, &Scene::rightEnd);

    connect(ui->menu, &Menu::save, this, &MainWindow::save);
    connect(ui->menu, &Menu::load, this, &MainWindow::load);
    connect(ui->menu, &Menu::save2, this, &MainWindow::save2);
    connect(ui->menu, &Menu::load2, this, &MainWindow::load2);

    connect(ui->menu, &Menu::start, scene, &Scene::start);
    connect(ui->menu, &Menu::start, this, &MainWindow::getRobotSettings);
    connect(ui->SaveButton, &QPushButton::clicked, this, &MainWindow::getRobotSettings);
    connect(ui->menu, &Menu::pause, scene, &Scene::pause);

    connect(ui->menu, &Menu::clear, this, &MainWindow::clear);

    connect(scene, &Scene::isPlayer, ui->menu, &Menu::isPlayer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    load2();
    exit(0);
}

Scene *MainWindow::initScene()
{
    Scene *scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setSceneRect(0, 0, 990, 690);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setResizeAnchor(QGraphicsView::NoAnchor);
    return scene;
}

RobotList *MainWindow::initRobotList()
{
    RobotList *robotList = new RobotList(ui->robotList);
    return robotList;
}

void MainWindow::save()
{
    saveFile();
}

void MainWindow::save2()
{
    saveFile(true);
}

void MainWindow::saveFile(bool saveStart)
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QString filename = "savedScene.json";
        if (saveStart)
        {
            filename = "tmpSavedScene.json";
        }

        QFile file(filename);
        if (file.exists() && !file.remove())
        {
            qDebug() << "Failed to remove existing file:" << file.errorString();
            return;
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }


        qint64 bytesWritten = file.write(scene->toJSON().toUtf8());
        if (bytesWritten == -1)
        {
            qDebug() << "Failed to write JSON data to file:" << file.errorString();
            return;
        }
        qDebug() << "Scene saved to file.";
        file.close();
    }
}

void MainWindow::load()
{
    loadFile();
}

void MainWindow::load2()
{
    loadFile(true);
}

void MainWindow::loadFile(bool loadStart)
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QString filename = "savedScene.json";
        if (loadStart)
        {
            filename = "tmpSavedScene.json";
        }

        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open file for reading:" << file.errorString();
            return;
        }

        scene->clear();
        scene->clearArraysAfterLoad();
        robotList->clearArray();

        QByteArray dataFromFile = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(dataFromFile);
        if (doc.isNull())
        {
            qDebug() << "Failed to parse JSON data";
            return;
        }
        if (!doc.isObject())
        {
            qDebug() << "JSON data is not an object";
            return;
        }

        QJsonObject obj = doc.object();
        QJsonArray arr = obj["items"].toArray();

        for (const QJsonValue &value : arr)
        {
            QJsonObject parameter = value.toObject();
            qreal x = parameter["x"].toDouble();
            qreal y = parameter["y"].toDouble();
            QString objectType = parameter["object"].toString();
            qreal rotation = parameter["rotation"].toDouble();
            qreal scale = parameter["scale"].toDouble();

            if (objectType == "Obstacle")
            {
                scene->spawnObstacleFromLoad(x, y, rotation, scale);
            }
            else if (objectType == "Robot")
            {
                scene->spawnRobotFromLoad(x, y, rotation);
            }
            else if (objectType == "Player")
            {
                scene->spawnRobotFromLoad(x, y, rotation, true);
            }
        }

        if (loadStart)
        {
            QFile::remove("tmpSavedScene.json");
        }
    }
}

void MainWindow::clear()
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        QFile file("tmpSavedScene.json");
        if (file.exists() && !file.remove())
        {
            qDebug() << "Failed to remove existing file:" << file.errorString();
            return;
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }
        file.close();
        load2();
    }
}

void MainWindow::getRobotSettings()
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        int robotCount = robotList->getRobotCount();
//        qDebug() << robotCount;
        for (int robotIndex = 0; robotIndex < robotCount; robotIndex++)
        {
            RobotSettings settings = robotList->getRobotSettings(robotIndex);

//            qDebug() << "Robot ID:" << settings.robotID;
//            qDebug() << "Distance:" << settings.distance;
//            qDebug() << "Rotation:" << settings.rotation;
//            qDebug() << "Rotation Direction:" << (settings.rotationDirection ? "Left" : "Right");

            bool ok;
            int robotID = settings.robotID.toInt(&ok);
            if (!ok)
            {
                qDebug() << "Coversion failed!";
                continue;
            }

            Robot *robot = scene->getRobot(robotID);
            if (robot)
            {
                //TODO
            }
        }
    }
}
