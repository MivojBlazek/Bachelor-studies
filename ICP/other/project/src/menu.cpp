#include "mainwindow.h"
#include "menu.h"
#include "ui_menu.h"
#include "scene.h"

#include <QPushButton>
#include <QDebug>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    connect(ui->SpawnObstacleButton, &QPushButton::clicked, this, &Menu::onSpawnObstacleButtonClicked);
    connect(ui->SpawnRobotButton, &QPushButton::clicked, this, &Menu::onSpawnRobotButtonClicked);
    connect(ui->SpawnPlayerButton, &QPushButton::clicked, this, &Menu::onSpawnPlayerButtonClicked);

    connect(ui->ForwardButton, &QPushButton::clicked, this, &Menu::onForwardButtonClicked);
    connect(ui->StopButton, &QPushButton::clicked, this, &Menu::onStopButtonClicked);
    connect(ui->LeftButton, &QPushButton::pressed, this, &Menu::leftButtonPressed);
    connect(ui->RightButton, &QPushButton::pressed, this, &Menu::rightButtonPressed);
    connect(ui->LeftButton, &QPushButton::released, this, &Menu::leftButtonReleased);
    connect(ui->RightButton, &QPushButton::released, this, &Menu::rightButtonReleased);

    connect(ui->SaveButton, &QPushButton::clicked, this, &Menu::onSaveButtonClicked);
    connect(ui->LoadButton, &QPushButton::clicked, this, &Menu::onLoadButtonClicked);

    connect(ui->StartButton, &QPushButton::clicked, this, &Menu::onStartButtonClicked);
    connect(ui->PauseButton, &QPushButton::clicked, this, &Menu::onPauseButtonClicked);
    connect(ui->ContinueButton, &QPushButton::clicked, this, &Menu::onContinueButtonClicked);
    connect(ui->ResetButton, &QPushButton::clicked, this, &Menu::onResetButtonClicked);
    connect(ui->EndButton, &QPushButton::clicked, this, &Menu::onEndButtonClicked);

    connect(ui->ClearButton, &QPushButton::clicked, this, &Menu::onClearButtonClicked);

    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->SpawnPlayerButton->setEnabled(true);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::onSpawnObstacleButtonClicked()
{
    emit spawnObstacle();
}

void Menu::onSpawnRobotButtonClicked()
{
    emit spawnRobot();
}

void Menu::onSpawnPlayerButtonClicked()
{
    emit spawnPlayer();
    ui->SpawnPlayerButton->setEnabled(false);
}

void Menu::onForwardButtonClicked()
{
    emit forward();
    ui->ForwardButton->setEnabled(false);
}

void Menu::onStopButtonClicked()
{
    emit stopPlayer();
    ui->ForwardButton->setEnabled(true);
}

void Menu::leftButtonPressed()
{
    emit left();
}

void Menu::rightButtonPressed()
{
    emit right();
}

void Menu::leftButtonReleased()
{
    emit leftEnd();
    ui->ForwardButton->setEnabled(true);
}

void Menu::rightButtonReleased()
{
    emit rightEnd();
    ui->ForwardButton->setEnabled(true);
}

void Menu::onSaveButtonClicked()
{
    emit save();
}

void Menu::onLoadButtonClicked()
{
    emit pause();
    emit load();

    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->ForwardButton->setEnabled(true);
}

void Menu::onStartButtonClicked()
{
    emit save2();
    emit start();

    ui->StartButton->hide();
    ui->PauseButton->show();
    ui->ContinueButton->hide();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->hide();
    ui->SpawnRobotButton->hide();
    ui->SpawnPlayerButton->hide();

    ui->StopButton->show();
    ui->ForwardButton->show();
    ui->LeftButton->show();
    ui->RightButton->show();

    ui->ClearButton->hide();
}

void Menu::onResetButtonClicked()
{
    emit pause();
    emit load2();

    ui->StartButton->show();
    ui->PauseButton->hide();
    ui->ContinueButton->hide();
    ui->ResetButton->hide();
    ui->EndButton->show();

    ui->SaveButton->show();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();

    ui->ForwardButton->setEnabled(true);
}

void Menu::onPauseButtonClicked()
{
    emit pause();

    ui->StartButton->hide();
    ui->PauseButton->hide();
    ui->ContinueButton->show();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->show();
    ui->SpawnRobotButton->show();
    ui->SpawnPlayerButton->show();

    ui->StopButton->hide();
    ui->ForwardButton->hide();
    ui->LeftButton->hide();
    ui->RightButton->hide();

    ui->ClearButton->show();
}

void Menu::onContinueButtonClicked()
{
    emit start();

    ui->StartButton->hide();
    ui->PauseButton->show();
    ui->ContinueButton->hide();
    ui->ResetButton->show();
    ui->EndButton->show();

    ui->SaveButton->hide();

    ui->SpawnObstacleButton->hide();
    ui->SpawnRobotButton->hide();
    ui->SpawnPlayerButton->hide();

    ui->StopButton->show();
    ui->ForwardButton->show();
    ui->LeftButton->show();
    ui->RightButton->show();

    ui->ClearButton->hide();
}

void Menu::onEndButtonClicked()
{
    emit load2();
    exit(0);
}

void Menu::onClearButtonClicked()
{
    ui->SpawnPlayerButton->setEnabled(true);
    emit clear();
}

void Menu::isPlayer()
{
    ui->SpawnPlayerButton->setEnabled(false);
}
