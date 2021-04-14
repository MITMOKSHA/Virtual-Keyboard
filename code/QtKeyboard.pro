QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtKeyboard
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x

CONFIG += c++11

win32: RC_ICONS += app.ico

INCLUDEPATH = Keyboard

SOURCES += main.cpp \
    Keyboard/KeyButton.cpp \
    Keyboard/Keyboard.cpp \
    Keyboard/styleseting.cpp

HEADERS  += \
    Keyboard/AbstractKeyboard.h \
    Keyboard/KeyButton.h \
    Keyboard/Keyboard.h \
    Keyboard/styleseting.h

RESOURCES += \
    Resources/Resources.qrc

FORMS += \
    Keyboard/styleseting.ui
