TEMPLATE     = app
TARGET       = 
DEPENDPATH  += . src
INCLUDEPATH += . src
QT          += opengl
LIBS        += -lboost_filesystem

# Input
HEADERS += src/block.hpp src/controller.hpp src/element.hpp src/ship.hpp src/vector.hpp src/window.hpp src/world.hpp
SOURCES += src/controller.cpp src/main.cpp src/ship.cpp src/window.cpp src/world.cpp
