#!/usr/bin/make
######################################
# configuration: change things here. #
######################################

PROGRAMS=SkywaysGlut SkywaysQt SkywaysSdl

SkywaysGlut_BINARY=skyways.glut
SkywaysQt_BINARY=skyways.qt
SkywaysSdl_BINARY=skyways.sdl

HEADERS= \
	src/configuration.hpp \
	src/controller.hpp \
	src/display/shader.hpp \
	src/display/textprinter.hpp \
	src/display/uniform.hpp \
	src/game.hpp \
	src/loading/blockloader.hpp \
	src/loading/mapgenerator.hpp \
	src/loading/maploader.hpp \
	src/loading/objmodel.hpp \
	src/range.hpp \
	src/vector.hpp \
	src/world/aabb.hpp \
	src/world/block.hpp \
	src/world/collisionaccelerator.hpp \
	src/world/element.hpp \
	src/world/map.hpp \
	src/world/model.hpp \
	src/world/ship.hpp \
	#

SkywasyGlut_HEADERS= $(HEADERS) \
	src/backends/configparser.hpp \
	#

SkywasyQt_HEADERS= $(HEADERS) \
	src/backends/qtconfigparser.hpp \
	src/backends/qtwindow.hpp \
	#

SkywaysSdl_HEADERS= $(HEADERS) \
	src/backends/configparser.hpp \
	#

CXXSOURCES= \
	src/configuration.cpp \
	src/controller.cpp \
	src/display/shader.cpp \
	src/display/textprinter.cpp \
	src/game.cpp \
	src/loading/blockloader.cpp \
	src/loading/mapgenerator.cpp \
	src/loading/maploader.cpp \
	src/loading/objmodel.cpp \
	src/world/block.cpp \
	src/world/collisionaccelerator.cpp \
	src/world/element.cpp \
	src/world/map.cpp \
	src/world/ship.cpp \
	#

SkywaysGlut_CXXSOURCES= $(CXXSOURCES) \
	src/backends/configparser.cpp \
	src/backends/glutmain.cpp \
	#

SkywaysQt_CXXSOURCES= $(CXXSOURCES) \
	src/backends/moc_qtwindow.cpp \
	src/backends/qtconfigparser.cpp \
	src/backends/qtmain.cpp \
	src/backends/qtwindow.cpp \
	#

SkywaysSdl_CXXSOURCES= $(CXXSOURCES) \
	src/backends/configparser.cpp \
	src/backends/sdlmain.cpp \
	#

CPPFLAGS:= -O2 -g -Wall -I/usr/include/FTGL -I/usr/include/freetype2 $(CPPFLAGS) -Isrc
LDFLAGS:= -lGL  -lboost_filesystem -lftgl -lGLEW $(LDFLAGS)

SkywaysGlut_LDFLAGS=-lglut -lboost_program_options
SkywaysQt_LDFLAGS=-lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread
SkywaysQt_CPPFLAGS=-D_REENTRANT -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4
SkywaysSdl_LDFLAGS=-lSDL -lboost_program_options
SkywaysSdl_CPPFLAGS=-I/usr/include/SDL

EXTRADIST=

######################################
# auto-configuration: do not change! #
######################################

CWD:=$(shell pwd)

default: all

$(foreach prog,$(PROGRAMS),$(eval $(prog)_SOURCES=$($(prog)_CSOURCES) $($(prog)_CXXSOURCES)))
$(foreach prog,$(PROGRAMS),$(eval $(prog)_OBJECTS=$(patsubst %.c,%_$(prog).o,$($(prog)_CSOURCES)) $(patsubst %.cpp,%_$(prog).o,$($(prog)_CXXSOURCES))))
$(foreach flag,CFLAGS CPPFLAGS CXXFLAGS LDFLAGS,$(foreach prog,$(PROGRAMS),$(eval $(prog)_ALL_$(flag)=$($(flag)) $($(prog)_$(flag)))))

#CSOURCES=$(foreach prog,$(PROGRAMS),$($(prog)_CSOURCES))
#CXXSOURCES=$(foreach prog,$(PROGRAMS),$($(prog)_CXXSOURCES))
#SOURCES=$(CSOURCES) $(CXXSOURCES)
#HEADERS=$(foreach prog,$(PROGRAMS),$($(prog)_HEADERS))
OBJECTS=$(foreach prog,$(PROGRAMS),$($(prog)_OBJECTS))
BINARIES=$(foreach prog,$(PROGRAMS),$($(prog)_BINARY))
DEPENDS=$(patsubst %.o,%.d,$(OBJECTS))

define CDEP_template
  %_$(1).d: %.c
	set -e; rm -f "$$@" || true; \
		$$(CC) -MM -MQ "`echo "$$<" | sed 's,\($$*\)\.c,\1_$(1).o,'`" $$($(1)_ALL_CPPFLAGS) $$($(1)_ALL_CFLAGS) "$$<" | \
		sed 's,\($$*_$(1)\).o *:,\1.o $$@:,' >$$@
endef

define CXXDEP_template
  %_$(1).d: %.cpp
	set -e; rm -f "$$@" || true; \
		$$(CXX) -MM -MQ "`echo "$$<" | sed 's,\($$*\)\.cpp,\1_$(1).o,'`" $$($(1)_ALL_CPPFLAGS) $$($(1)_ALL_CXXFLAGS) "$$<" | \
		sed 's,\($$*_$(1)\).o *:,\1.o $$@:,' >$$@
endef

define CSRC_template
  %_$(1).o: %.c
	$$(CC) -c $$($(1)_ALL_CPPFLAGS) $$($(1)_ALL_CFLAGS) $$< -o $$@
endef

define CXXSRC_template
  %_$(1).o: %.cpp
	$$(CXX) -c $$($(1)_ALL_CPPFLAGS) $$($(1)_ALL_CXXFLAGS) $$< -o $$@
endef

define BINARY_template
  $$($(1)_BINARY): $$($(1)_OBJECTS)
	$$(CC) $$($(1)_ALL_LDFLAGS) $$^ $$(LOADLIBS) $$(LDLIBS) -o $$@
endef

$(foreach template,CDEP CXXDEP CSRC CXXSRC BINARY,$(foreach prog,$(PROGRAMS),$(eval $(call $(template)_template,$(prog)))))

all: $(BINARIES)

#################################
# extra stuff: change as needed #
#################################

clean:
	rm $(OBJECTS) $(BINARIES) $(DEPENDS) $(DISTFILES) || true

moc_%.cpp: %.hpp
	moc-qt4 $(SkywaysQt_CPPFLAGS) $(SkywaysQt_CXXFLAGS) $< -o $@

.PHONY: default clean

-include $(DEPENDS)
