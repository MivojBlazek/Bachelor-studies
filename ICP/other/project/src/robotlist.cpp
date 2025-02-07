#include "robotlist.h"
#include "ui_robotlist.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>

RobotList::RobotList(QWidget *parent) :
    QWidget(parent),
    i(0),
    ui(new Ui::RobotList)
{
    ui->setupUi(this);
}

void RobotList::clearArray()
{
    for (auto widget : robotWidgets)
    {
        delete widget->findChild<QLabel *>("robotIDLabel");
        delete widget->findChild<QSlider *>("distanceSlider");
        delete widget->findChild<QLineEdit *>("distanceLineEdit");
        delete widget->findChild<QSlider *>("rotationSlider");
        delete widget->findChild<QLineEdit *>("rotationLineEdit");
        delete widget->findChild<QRadioButton *>("rotationRadioLeft");
        delete widget->findChild<QRadioButton *>("rotationRadioRight");
        delete widget->findChild<QHBoxLayout *>();
        delete widget->findChild<QWidget *>();
    }
    robotWidgets.clear();
    i = 0;
}

void RobotList::spawnRobot()
{
    QWidget *settingsWidget = new QWidget(this);
    QHBoxLayout *settingsLayout = new QHBoxLayout(settingsWidget);
    QLabel *robotID = new QLabel(QString::number(i), settingsWidget);
    QSlider *distanceSlider = new QSlider(Qt::Horizontal, settingsWidget);
    QLineEdit *distanceLineEdit = new QLineEdit(settingsWidget);
    QSlider *rotationSlider = new QSlider(Qt::Horizontal, settingsWidget);
    QLineEdit *rotationLineEdit = new QLineEdit(settingsWidget);
    QRadioButton *rotationRadioLeft = new QRadioButton(settingsWidget);
    QRadioButton *rotationRadioRight = new QRadioButton(settingsWidget);

    robotID->setObjectName("robotIDLabel");
    distanceSlider->setObjectName("distanceSlider");
    distanceLineEdit->setObjectName("distanceLineEdit");
    rotationSlider->setObjectName("rotationSlider");
    rotationLineEdit->setObjectName("rotationLineEdit");
    rotationRadioLeft->setObjectName("rotationRadioLeft");
    rotationRadioRight->setObjectName("rotationRadioRight");


    QButtonGroup *rotationButtonGroup = new QButtonGroup(settingsWidget);
    rotationButtonGroup->addButton(rotationRadioLeft);
    rotationButtonGroup->addButton(rotationRadioRight);
    rotationRadioLeft->setChecked(true);

    distanceSlider->setFixedWidth(100);
    rotationSlider->setFixedWidth(100);


    connect(distanceSlider, &QSlider::valueChanged, this, [distanceLineEdit](int value) {
        distanceLineEdit->setText(QString::number(value));
    });

    connect(rotationSlider, &QSlider::valueChanged, this, [rotationLineEdit](int value) {
        rotationLineEdit->setText(QString::number(value));
    });
    connect(distanceLineEdit, &QLineEdit::textChanged, this, [distanceSlider](const QString &text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok) {
            distanceSlider->setValue(value);
        }
    });

    connect(rotationLineEdit, &QLineEdit::textChanged, this, [rotationSlider](const QString &text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok) {
            rotationSlider->setValue(value);
        }
    });

    RobotSettings settings;
    settings.robotID = QString::number(i++);
    settings.distance = distanceSlider->value();
    settings.rotation = rotationSlider->value();
    settings.rotationDirection = rotationRadioLeft->isChecked(); // true for left, false for right
    settingsList.append(settings);

    settingsLayout->addWidget(robotID);
    settingsLayout->addWidget(distanceSlider);
    settingsLayout->addWidget(distanceLineEdit);
    settingsLayout->addWidget(rotationSlider);
    settingsLayout->addWidget(rotationLineEdit);
    settingsLayout->addWidget(rotationRadioLeft);
    settingsLayout->addWidget(rotationRadioRight);

    settingsWidget->setLayout(settingsLayout);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(this);
        setLayout(mainLayout);
    }
    mainLayout->addWidget(settingsWidget);
    robotWidgets.append(settingsWidget);

    connect(distanceSlider, &QSlider::valueChanged, this, &RobotList::update);
    connect(rotationSlider, &QSlider::valueChanged, this, &RobotList::update);
    connect(distanceLineEdit, &QLineEdit::textChanged, this, &RobotList::update);
    connect(rotationLineEdit, &QLineEdit::textChanged, this, &RobotList::update);
    connect(rotationButtonGroup, &QButtonGroup::idClicked, this, &RobotList::update);}

RobotSettings RobotList::getRobotSettings(int robotIndex) const
{
   return settingsList[robotIndex];
}

void RobotList::update()
{
    for (int i = 0; i < robotWidgets.size(); ++i) {
        QWidget *widget = robotWidgets[i];
        QSlider *distanceSlider = widget->findChild<QSlider *>("distanceSlider");
        QLineEdit *distanceLineEdit = widget->findChild<QLineEdit *>("distanceLineEdit");
        QSlider *rotationSlider = widget->findChild<QSlider *>("rotationSlider");
        QLineEdit *rotationLineEdit = widget->findChild<QLineEdit *>("rotationLineEdit");
        QRadioButton *rotationRadioLeft = widget->findChild<QRadioButton *>("rotationRadioLeft");
        QRadioButton *rotationRadioRight = widget->findChild<QRadioButton *>("rotationRadioRight");

        if (distanceSlider && distanceLineEdit && rotationSlider && rotationLineEdit &&
            rotationRadioLeft && rotationRadioRight) {
            settingsList[i].distance = distanceSlider->value();
            settingsList[i].rotation = rotationSlider->value();
            settingsList[i].rotationDirection = rotationRadioLeft->isChecked();
        }

    }
}

int RobotList::getRobotCount()
{
    return settingsList.size();
}

RobotList::~RobotList()
{
    delete ui;
}
