QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Checkers
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Game.cpp \
    main.cpp \
    BackTracking.cpp \


HEADERS += \
    Game.h \
    main.h \
    BackTracking.h \
    Square.h \
    Check.h \
    MovePiece.h
