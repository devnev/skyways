#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

DATA_$(sp) := $(d)/world $(d)/DejaVuSans.ttf
DOCS_$(sp) := $(d)/COPYING $(d)/README
SUBDIRS := blocks shaders src

$(eval $(call leave_directory))

# vim: ft=make
