QT += core
QT -= gui

TARGET = file_sorting
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    input_creator.cpp

CONFIG += c++11
