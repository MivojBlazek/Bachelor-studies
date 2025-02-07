#ifndef ROBOTLIST_H
#define ROBOTLIST_H

#include <QWidget>

namespace Ui {
class RobotList;
}

struct RobotSettings {
    QString robotID;
    int distance;
    int rotation;
    bool rotationDirection; // true for left, false for right
};

class RobotList : public QWidget
{
    Q_OBJECT

public:
    explicit RobotList(QWidget *parent = nullptr);
    ~RobotList();

    int getRobotCount();
    RobotSettings getRobotSettings(int robotIndex) const;

public slots:
    void spawnRobot();
    void update();
    void clearArray();

signals:
    void settingsChanged();

private:
    int i;

    Ui::RobotList *ui;
    QList<QWidget *> robotWidgets;
    QList<RobotSettings> settingsList;
};

#endif // ROBOTLIST_H
