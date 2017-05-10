QT += core
QT -= gui

CONFIG += c++11

TARGET = img-graph-gray
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/igraph

LIBS += `pkg-config opencv --libs`
LIBS += -L/usr/local/lib -ligraph





SOURCES += main.cpp \
    vectorgraph.cpp

HEADERS += \
    vectorgraph.hpp
