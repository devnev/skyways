#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))

DEPDIR?=.deps

#{{{ per-directory-and-module source listing hooks

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
MODDIR_TEMPLATES := $(MODDIR_TEMPLATES) csrcall cxxsrcall hdrall objects

#}}}
#{{{ per-module compilation flag hooks

define FLAGS_module_tpl
  $$(foreach flag,CFLAGS CXXFLAGS CPPFLAGS LDFLAGS LIBS,$$(eval $$(flag)_$(1):=$$($$(flag)) $$($(1)_$$(flag)) $$($$(flag)_$(1))))
endef
define C_module_tpl
  %_$(1).o: %.c
	$$(CC) $$(CFLAGS_$(1)) $$(CPPFLAGS_$(1)) -MT $$@ -MMD -MP -MF $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td -c -o $$@ $$<
	mv -f $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).d
endef
define CXX_module_tpl
  %_$(1).o: %.cpp
	$$(CXX) $$(CXXFLAGS_$(1)) $$(CPPFLAGS_$(1)) -MT $$@ -MMD -MP -MF $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td -c -o $$@ $$<
	mv -f $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).Td $$(dir $$*)$$(DEPDIR)/$$(notdir $$*)_$(1).d
endef
define LD_module_tpl
  $$($(1)_BINARY): $$(OBJECTS_$(1))
	$$(LINK.o) $$(LDFLAGS_$(1)) $$(LIBS_$(1)) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@
endef
MOD_TEMPLATES := $(MOD_TEMPLATES) FLAGS C CXX LD

#}}}
#{{{ post-module combination lists

define targets_postmod_tpl
  OBJECTS:=$$(foreach mod,$(MODULES),$$(OBJECTS_$$(mod)))
  BINARIES:=$$(foreach mod,$(MODULES),$$($$(mod)_BINARY))
  #DEPENDS:=$$(foreach obj,$$(OBJECTS),$$(dir $$(obj))$$(DEPDIR)/$$(basename $$(obj)).d)
  DEPENDS:=$$(join $$(dir $$(OBJECTS)),$$(addprefix $$(DEPDIR)/,$$(addsuffix .d,$$(basename $$(notdir $$(OBJECTS))))))
  CLEAN:=$$(CLEAN) $$(OBJECTS) $$(BINARIES) $$(DEPENDS)
endef
POSTMOD_TEMPLATES := $(POSTMOD_TEMPLATES) targets

#}}}
#{{{ dependency rule

define depends_rules_tpl
  -include $$(DEPENDS)
  all: $$(BINARIES)
  $(OBJECTS): build-dirs deps-dirs
  .PHONY: build-dirs deps-dirs
  build-dirs:
	mkdir -p $(patsubst %,$(builddir)/%,$(DIRECTORIES))
  deps-dirs:
	mkdir -p $(patsubst %,$(builddir)/%/$(DEPDIR),$(DIRECTORIES))
endef
RULES_TEMPLATES := $(RULES_TEMPATE) depends

#}}}
