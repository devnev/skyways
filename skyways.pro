TEMPLATE     = app
TARGET       = 
DEPENDPATH  += . src
INCLUDEPATH += . src
QT          += opengl
LIBS        += -lboost_filesystem

# Input
HEADERS += src/block.hpp src/controller.hpp src/element.hpp src/ship.hpp src/vector.hpp src/qtwindow.hpp src/world.hpp
SOURCES += src/block.cpp src/controller.cpp src/qtmain.cpp src/ship.cpp src/qtwindow.cpp src/world.cpp

# vim: ft=dosini
