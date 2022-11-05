QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Checkers
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Game.cpp \
    main.cpp \


HEADERS += \
    Game.h \
    main.h \

