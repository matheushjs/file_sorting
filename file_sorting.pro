QT += core
QT -= gui

TARGET = file_sorting
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
	input_creator.cpp \
	mydatabase.cpp

CONFIG += c++11

HEADERS += \
	mydatabase.h

LIBS += -pthread

#QMAKE_CXXFLAGS += -O3
