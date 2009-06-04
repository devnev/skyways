#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CXXSOURCES_$(sp) := $(d)/blockloader.cpp $(d)/mapgenerator.cpp $(d)/maploader.cpp $(d)/objmodel.cpp
HEADERS_$(sp) := $(d)/blockloader.hpp $(d)/mapgenerator.hpp $(d)/maploader.hpp $(d)/objmodel.hpp

$(eval $(call leave_directory))

# vim: ft=make
