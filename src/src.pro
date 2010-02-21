TEMPLATE = app
TARGET = Editor
DESTDIR = ../bin

CONFIG += console

INCLUDEPATH += "C:\Program Files\Lua\5.1\include"
LIBS += "C:\Program Files\Lua\5.1\lib\lua51.lib"

SOURCES += main.cpp \
           mainwindow.cpp \
           highlighter.cpp \
           codeeditor.cpp \
           PluginManager.cpp
HEADERS += mainwindow.h \
          highlighter.h \
          ui_mainwindow.h \
          codeeditor.h \
          PluginManager.h \
          PluginManager_p.h \
          luna.h
RESOURCES +=
