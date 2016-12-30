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
    GUI/options.cpp \
    GUI/tabacces.cpp \
    GUI/tabbar.cpp \
    GUI/Tabs/welcome.cpp \
    Game/mappainter.cpp \
    GUI/Tabs/Docks/bdock.cpp \
    GUI/Tabs/Docks/intertie.cpp \
    GUI/Tabs/Docks/bdockszone.cpp \
    GUI/Tabs/Docks/bdockwidget.cpp \
    GUI/Tabs/bcolor.cpp \
    GUI/Tabs/tabwidget.cpp \
    GUI/Tabs/itemdelegates.cpp \
    GUI/quietwidgets.cpp \
    Game/gameobject.cpp \
    Game/ItemModels/itemmodels.cpp \
    Game/ItemModels/mapslistmodel.cpp \
    Game/ItemModels/typeitemmodel.cpp \
    Game/ItemModels/attrtreeitemmodel.cpp \
    GUI/Tabs/treeviews.cpp \
    GUI/Tabs/ObjectEditors/gameobjecteditor.cpp \
    GUI/Tabs/ObjectEditors/celltypeeditor.cpp \
    GUI/Tabs/worldtab.cpp \
    GUI/Tabs/objecttab.cpp \
    GUI/Tabs/ObjectEditors/mapresizedialog.cpp \
    GUI/Tabs/MapTab/mapviewer.cpp \
    GUI/Tabs/MapTab/maptab.cpp \
    GUI/Tabs/MapTab/celldock.cpp \
    GUI/Tabs/MapTab/selectiondock.cpp \
    GUI/Tabs/MapTab/celltypesdock.cpp \
    GUI/Tabs/MapTab/mapdock.cpp \
    GUI/Tabs/ObjectEditors/mapeditor.cpp

HEADERS  += \
    Game/game.h \
    Game/xmlhandler.h \
    Game/object.h \
    Game/map.h \
    GUI/editor.h \
    GUI/newgame.h \
    GUI/options.h \
    GUI/tabacces.h \
    GUI/tabbar.h \
    GUI/Tabs/welcome.h \
    Game/mappainter.h \
    GUI/Tabs/Docks/bdock.h \
    GUI/Tabs/Docks/intertie.h \
    GUI/Tabs/Docks/bdockszone.h \
    GUI/Tabs/Docks/bdockwidget.h \
    GUI/Tabs/bcolor.h \
    GUI/Tabs/tabwidget.h \
    GUI/Tabs/itemdelegates.h \
    GUI/quietwidgets.h \
    Game/gameobject.h \
    Game/typedobject.hxx \
    Game/ItemModels/itemmodels.h \
    Game/ItemModels/mapslistmodel.h \
    Game/ItemModels/typeitemmodel.h \
    Game/ItemModels/attrtreeitemmodel.h \
    GUI/Tabs/treeviews.h \
    GUI/Tabs/ObjectEditors/gameobjecteditor.h \
    GUI/Tabs/ObjectEditors/celltypeeditor.h \
    GUI/Tabs/worldtab.h \
    GUI/Tabs/objecttab.h \
    GUI/Tabs/ObjectEditors/mapresizedialog.h \
    GUI/Tabs/MapTab/celldock.h \
    GUI/Tabs/MapTab/celltypesdock.h \
    GUI/Tabs/MapTab/mapdock.h \
    GUI/Tabs/MapTab/maptab.h \
    GUI/Tabs/MapTab/mapviewer.h \
    GUI/Tabs/MapTab/selectiondock.h \
    GUI/Tabs/ObjectEditors/mapeditor.h

FORMS    += \
    GUI/editor.ui \
    GUI/newgame.ui \
    GUI/tabacces.ui \
    GUI/Tabs/welcome.ui \
    GUI/Tabs/ObjectEditors/celltypeeditor.ui \
    GUI/Tabs/worldtab.ui \
    GUI/Tabs/objecttab.ui \
    GUI/Tabs/ObjectEditors/mapresizedialog.ui \
    GUI/Tabs/MapTab/celltypesdock.ui \
    GUI/Tabs/MapTab/mapdock.ui \
    GUI/Tabs/MapTab/maptab.ui \
    GUI/Tabs/MapTab/selectiondock.ui \
    GUI/Tabs/MapTab/celldock.ui \
    GUI/Tabs/ObjectEditors/mapeditor.ui

RESOURCES += \
    Icons/ressources.qrc

DISTFILES +=
