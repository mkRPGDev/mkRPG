#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T20:26:37
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RPGEditor
TEMPLATE = app


SOURCES += main.cpp\
    Game/game.cpp \
    Game/xmlhandler.cpp \
    Game/object.cpp \
    Game/map.cpp \
    GUI/editor.cpp \
    GUI/newgame.cpp \
    GUI/Tabs/mapseditor.cpp \
    GUI/Tabs/mapviewer.cpp \
    GUI/Tabs/worldeditor.cpp \
    GUI/options.cpp \
    GUI/tabacces.cpp \
    GUI/tabbar.cpp \
    GUI/Tabs/welcome.cpp \
    Game/mapslistmodel.cpp

HEADERS  += \
    Game/game.h \
    Game/xmlhandler.h \
    Game/object.h \
    Game/map.h \
    GUI/editor.h \
    GUI/newgame.h \
    GUI/Tabs/mapseditor.h \
    GUI/Tabs/mapviewer.h \
    GUI/Tabs/worldeditor.h \
    GUI/options.h \
    GUI/tabacces.h \
    GUI/tabbar.h \
    GUI/Tabs/welcome.h \
    Game/mapslistmodel.h

FORMS    += \
    GUI/editor.ui \
    GUI/newgame.ui \
    GUI/Tabs/mapseditor.ui \
    GUI/Tabs/worldeditor.ui \
    GUI/tabacces.ui \
    GUI/Tabs/welcome.ui

RESOURCES += \
    Icons/ressources.qrc
