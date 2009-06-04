#!/usr/bin/make

default: all

MK_INCLUDE:=1
SRCDIR:=.
BUILDDIR:=.
DEPDIR:=.deps

#{{{ load configuration
#{{{ include build configuration

include $(BUILDDIR)/Config.mk

#}}}
#{{{ directory enter and leave hooks

define checkdir_enter_tpl
  $$(if $$(directory),,$$(error Set the `directory` variable before including this makefile in other makefiles))
endef
define enter_enter_tpl
  supsp := $$(sp)
  sp := $$(lastsp).x
  lastsp := $$(sp)
  sp_list := $$(sp_list) $$(sp)
  dirstack_$$(sp) := $$(d)
  d := $$(directory)
  DIRECTORIES := $$(DIRECTORIES) $$(d)
endef
define subdirs_enter_tpl
  SUBDIRS :=
endef
ENTER_TEMPLATES := checkdir enter subdirs $(ENTER_TEMPLATES)
define enter_directory
  $(foreach tpl,$(ENTER_TEMPLATES),$$(eval $$(call $(tpl)_enter_tpl)))
endef

define include_subdir
  directory := $$(d)/$(1)
  include $$(SRCDIR)/$$(directory)/Dir.mk
endef
define include_subdir_list
  $$(foreach subdir,$(1),$$(eval $$(call include_subdir,$$(subdir))))
endef
define subdirs_leave_tpl
  SUBDIRS_$$(sp) := $$(SUBDIRS)
  $$(if $$(SUBDIRS_$$(sp)),$$(eval $$(call include_subdir_list,$$(SUBDIRS_$$(sp)))),)
  SUBDIRS :=
endef
define leave_leave_tpl
  d := $$(dirstack_$$(sp))
  sp := $$(supsp)
endef
LEAVE_TEMPLATES := $(LEAVE_TEMPLATES) subdirs leave
define leave_directory
  $(foreach tpl,$(LEAVE_TEMPLATES),$$(eval $$(call $(tpl)_leave_tpl)))
endef

#}}}
#{{{ include top directory configuration

directory := .
include $(SRCDIR)/Dir.mk

#}}}
#}}}
#{{{ process configuration
#{{{ per-directory-and-module hooks

define csrcall_moddir_tpl
  CSOURCES_$(1)_$(2):=$$(CSOURCES_$(1)_$(2)) $$(CSOURCES_$(2))
  CSOURCES_$(1):=$$(CSOURCES_$(1)) $$(CSOURCES_$(1)_$(2))
endef
define cxxsrcall_moddir_tpl
  CXXSOURCES_$(1)_$(2):=$$(CXXSOURCES_$(1)_$(2)) $$(CXXSOURCES_$(2))
  CXXSOURCES_$(1):=$$(CXXSOURCES_$(1)) $$(CXXSOURCES_$(1)_$(2))
endef
define hdrall_moddir_tpl
  HEADERS_$(1)_$(2):=$$(HEADERS_$(1)_$(2)) $$(HEADERS_$(2))
  HEADERS_$(1):=$$(HEADERS_$(1)) $$(HEADERS_$(1)_$(2))
endef
define objects_moddir_tpl
  OBJECTS_$(1)_$(2):=$$(OBJECTS_$(1)_$(2)) $$(patsubst %.c,%_$(1).o,$$(CSOURCES_$(1)_$(2)))
  OBJECTS_$(1)_$(2):=$$(OBJECTS_$(1)_$(2)) $$(patsubst %.cpp,%_$(1).o,$$(CXXSOURCES_$(1)_$(2)))
  OBJECTS_$(1):=$$(OBJECTS_$(1)) $$(OBJECTS_$(1)_$(2))
endef
MODDIR_TEMPLATES := $(MODDIR_TEMPLATES) \
	csrcall_moddir_tpl cxxsrcall_moddir_tpl hdrall_moddir_tpl objects_moddir_tpl
define process_module_directory
  $$(foreach tpl,$$(MODDIR_TEMPLATES),$$(eval $$(call $$(tpl),$(1),$(2))))
endef

#}}}
#{{{ per-modules hooks

define FLAGS_mod_template
  $$(foreach flag,CFLAGS CXXFLAGS CPPFLAGS LDFLAGS LIBS,$$(eval $$(flag)_$(1):=$$($$(flag)) $$($(1)_$$(flag)) $$($$(flag)_$(1))))
endef
define C_mod_template
  %_$(1).o: %.c
	$$(CC) $$(CFLAGS_$(1)) $$(CPPFLAGS_$(1)) -MT $$@ -MD -MP -MF $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td -c -o $$@ $$<
	mv -f $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).d
endef
define CXX_mod_template
  %_$(1).o: %.cpp
	$$(CXX) $$(CXXFLAGS_$(1)) $$(CPPFLAGS_$(1)) -MT $$@ -MD -MP -MF $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td -c -o $$@ $$<
	mv -f $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).d
endef
define LD_mod_template
  $$($(1)_BINARY): $$(OBJECTS_$(1))
	$$(LINK.o) $$(LDFLAGS_$(1)) $$(LIBS_$(1)) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@
endef
MOD_TEMPLATES := $(MOD_TEMPLATES) FLAGS_mod_template C_mod_template CXX_mod_template LD_mod_template
define process_module
  $$(foreach sp,$$(sp_list),$$(eval $$(call process_module_directory,$(1),$$(sp))))
  $$(foreach tpl,$$(MOD_TEMPLATES),$$(eval $$(call $$(tpl),$(1))))
endef

#}}}
#{{{ process module list

define process_modules
  $$(foreach mod,$(1),$$(eval $$(call process_module,$$(mod))))
  OBJECTS:=$$(foreach mod,$(1),$$(OBJECTS_$$(mod)))
  BINARIES:=$$(foreach mod,$(1),$$($$(mod)_BINARY))
  #DEPENDS:=$$(foreach obj,$$(OBJECTS),$$(dir $$(obj))$$(DEPDIR)/$$(basename $$(obj)).d)
  DEPENDS:=$$(join $$(dir $$(OBJECTS)),$$(addprefix $$(DEPDIR)/,$$(addsuffix .d,$$(basename $$(notdir $$(OBJECTS))))))
endef
$(eval $(call process_modules,$(MODULES)))

#}}}
#}}}
#{{{ misc

-include $(DEPENDS)

CLEAN:=$(CLEAN) $(OBJECTS) $(BINARIES) $(DEPENDS)

.PHONY: default all
all: $(BINARIES)

.PHONY: clean
clean:
	rm -f $(CLEAN) || true

.PHONY: force
force:
	@/bin/true

$(SRCDIR)/Rules.mk: force
	@mkdir -p $(patsubst %,$(BUILDDIR)/%,$(DIRECTORIES)) $(patsubst %,$(BUILDDIR)/%/$(DEPDIR),$(DIRECTORIES))

#}}}
# vim: fdm=marker
