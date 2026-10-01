QT -= gui
QT += core

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = ConsoleCalculator
TEMPLATE = app

SOURCES += \
    main.cpp \
    calculator.cpp \
    history.cpp

HEADERS += \
    calculator.h \
    history.h

DISTFILES += \
    .gitignore \
    README.md