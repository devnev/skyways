#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

DATA_$(sp) := $(d)/cube $(d)/flat $(d)/tunnel

$(eval $(call leave_directory))

# vim: ft=make
