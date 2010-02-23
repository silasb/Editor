TEMPLATE = app
TARGET = Editor
DESTDIR = ../bin

SOURCES += main.cpp \
           mainwindow.cpp \
           highlighter.cpp \
           codeeditor.cpp
HEADERS += mainwindow.h \
          highlighter.h \
          ui_mainwindow.h \
          codeeditor.h
RESOURCES +=

win32:debug {
  CONFIG += console
}
