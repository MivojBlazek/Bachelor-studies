QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTS_DIR = ./obj

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    obstacle.cpp \
    robot.cpp \
    robotlist.cpp \
    scene.cpp

HEADERS += \
    mainwindow.h \
    menu.h \
    obstacle.h \
    robot.h \
    robotlist.h \
    scene.h

FORMS += \
    mainwindow.ui \
    menu.ui \
    robotlist.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
