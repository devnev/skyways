TEMPLATE     = app
TARGET       = 
DEPENDPATH  += . src
INCLUDEPATH += . src
QT          += opengl

# Input
HEADERS += src/controller.hpp src/ship.hpp src/window.hpp
SOURCES += src/controller.cpp src/main.cpp src/ship.cpp src/window.cpp
