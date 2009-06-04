#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CSOURCES_$(sp) :=
CXXSOURCES_$(sp) :=
HEADERS_$(sp) :=
CSOURCES_module_$(sp) :=
CXXSOURCES_module_$(sp) :=
HEADERS_module_$(sp) :=
SUBDIRS :=

$(eval $(call leave_directory))

# vim: ft=make
