#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CXXSOURCES_$(sp) := $(d)/configuration.cpp $(d)/controller.cpp $(d)/game.cpp
HEADERS_$(sp) := $(d)/configuration.hpp $(d)/controller.hpp $(d)/game.hpp $(d)/range.hpp $(d)/vector.hpp

SUBDIRS := backends display loading world

$(eval $(call leave_directory))

# vim: ft=make
