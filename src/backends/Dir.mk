#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))
$(eval $(call enter_directory))

CXXSOURCES_SkywaysQt_$(sp) := $(d)/qtconfigparser.cpp $(d)/qtmain.cpp $(d)/qtwindow.cpp
CXXSOURCES_SkywaysSdl_$(sp) := $(d)/configparser.cpp $(d)/sdlmain.cpp
CXXSOURCES_SkywaysGlut_$(sp) := $(d)/configparser.cpp $(d)/glutmain.cpp
HEADERS_SkywaysQt_$(sp) := $(d)/qtconfigparser.hpp $(d)/qtwindow.hpp
HEADERS_SkywaysSdl_$(sp) := $(d)/configparser.hpp
HEADERS_SkywaysGlut_$(sp) := $(d)/configparser.hpp
OBJECTS_SkywaysQt_$(sp) := $(d)/moc_qtwindow_SkywaysQt.o

moc_%.cpp: %.hpp
	moc-qt4 $(SkywaysQt_CPPFLAGS) $(SkywaysQt_CXXFLAGS) $< -o $@

$(eval $(call leave_directory))

# vim: ft=make
