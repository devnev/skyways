#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CXXSOURCES_$(sp) := $(d)/shader.cpp $(d)/textprinter.cpp
HEADERS_$(sp) := $(d)/shader.hpp $(d)/textprinter.hpp $(d)/uniform.hpp

$(eval $(call leave_directory))

# vim: ft=make
