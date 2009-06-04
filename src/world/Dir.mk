#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CXXSOURCES_$(sp) := $(d)/block.cpp $(d)/collisionaccelerator.cpp $(d)/element.cpp $(d)/map.cpp $(d)/ship.cpp
HEADERS_$(sp) := $(d)/aabb.hpp $(d)/block.hpp $(d)/collisionaccelerator.hpp $(d)/element.hpp $(d)/map.hpp $(d)/model.hpp $(d)/ship.hpp

$(eval $(call leave_directory))

# vim: ft=make
