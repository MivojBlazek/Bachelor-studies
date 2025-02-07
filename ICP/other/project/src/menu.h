#ifndef MENU_H
#define MENU_H

#include <QWidget>

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

    void isPlayer();

signals:
    void spawnObstacle();
    void spawnRobot();
    void spawnPlayer();

    void forward();
    void stopPlayer();
    void left();
    void right();
    void leftEnd();
    void rightEnd();

    void save();
    void load();
    void save2();
    void load2();

    void pause();
    void start();
    void end();

    void clear();

private slots:
    void onSpawnObstacleButtonClicked();
    void onSpawnRobotButtonClicked();
    void onSpawnPlayerButtonClicked();

    void onForwardButtonClicked();
    void onStopButtonClicked();
    void leftButtonPressed();
    void rightButtonPressed();
    void leftButtonReleased();
    void rightButtonReleased();

    void onSaveButtonClicked();
    void onLoadButtonClicked();

    void onStartButtonClicked();
    void onResetButtonClicked();
    void onPauseButtonClicked();
    void onContinueButtonClicked();
    void onEndButtonClicked();

    void onClearButtonClicked();

private:
    Ui::Menu *ui;
};

#endif // MENU_H
