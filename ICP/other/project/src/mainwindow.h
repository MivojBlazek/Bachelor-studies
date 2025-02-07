#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "scene.h"
#include "robot.h"
#include "robotlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *event) override;
    void getRobotSettings();

private:
    Scene *initScene();
    RobotList *initRobotList();
    RobotList *robotList;

    void save();
    void load();
    void save2();
    void load2();
    void saveFile(bool saveStart = false);
    void loadFile(bool loadStart = false);

    void clear();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
