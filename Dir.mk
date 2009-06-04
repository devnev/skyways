#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

SUBDIRS := src

$(eval $(call leave_directory))

# vim: ft=make
