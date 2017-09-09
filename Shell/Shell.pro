TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lreadline
QMAKE_CXXFLAGS += -O2
SOURCES += main.cpp \
    inputmanager.cpp \
    environment.cpp \
    command.cpp

HEADERS += \
    inputmanager.h \
    environment.h \
    command.h

