
###########################
# prelude: do not change! #
###########################

CWD:=$(shell pwd)

default: all

BUILDFLAGS=CFLAGS CPPFLAGS CXXFLAGS LDFLAGS

define FLAGINIT_template
  ifndef $(1)
    $(1)=
  endif
endef

$(foreach flag,$(BUILDFLAGS),$(eval $(call FLAGINIT_template,$(flag))))

######################################
# configuration: change things here. #
######################################

PROGRAMS=SkywaysGlut SkywaysQt SkywaysSdl

SkywaysGlut_BINARY=skyways.glut
SkywaysQt_BINARY=skyways.qt
SkywaysSdl_BINARY=skyways.sdl

HEADERS= \
	src/aabb.hpp \
	src/block.hpp \
	src/configuration.hpp \
	src/controller.hpp \
	src/element.hpp \
	src/model.hpp \
	src/objmodel.hpp \
	src/ship.hpp \
	src/textprinter.hpp \
	src/vector.hpp \
	src/map.hpp

SkywasyGlut_HEADERS= $(HEADERS) \
	src/configparser.hpp

SkywasyQt_HEADERS= $(HEADERS) \
	src/qtconfigparser.hpp \
	src/qtwindow.hpp

SkywaysSdl_HEADERS= $(HEADERS) \
	src/configparser.hpp

CXXSOURCES= \
	src/block.cpp \
	src/configuration.cpp \
	src/controller.cpp \
	src/element.cpp \
	src/objmodel.cpp \
	src/ship.cpp \
	src/textprinter.cpp \
	src/map.cpp

SkywaysGlut_CXXSOURCES= $(CXXSOURCES) \
	src/configparser.cpp \
	src/glutmain.cpp

SkywaysQt_CXXSOURCES= $(CXXSOURCES) \
	src/moc_qtwindow.cpp \
	src/qtconfigparser.cpp \
	src/qtmain.cpp \
	src/qtwindow.cpp

SkywaysSdl_CXXSOURCES= $(CXXSOURCES) \
	src/configparser.cpp \
	src/sdlmain.cpp

CPPFLAGS+= -O2 -g -Wall -I/usr/include/FTGL -I/usr/include/freetype2
LDFLAGS+= -lGL  -lboost_filesystem -lftgl

SkywaysGlut_LDFLAGS=-lglut -lboost_program_options
SkywaysQt_LDFLAGS=-lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread
SkywaysQt_CPPFLAGS=-D_REENTRANT -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4
SkywaysSdl_LDFLAGS=-lSDL -lboost_program_options
SkywaysSdl_CPPFLAGS=-I/usr/include/SDL

EXTRADIST=

######################################
# auto-configuration: do not change! #
######################################

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
