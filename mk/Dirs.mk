#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))

ifndef DIRS_MK_INCLUDED
DIRS_MK_INCLUDED := 1

#{{{ directory enter hooks

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
define enter_directory
  $(foreach tpl,checkdir enter subdirs $(ENTER_TEMPLATES),$$(eval $$(call $(tpl)_enter_tpl)))
endef

#}}}
#{{{ directory leave hooks

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
define leave_directory
  $(foreach tpl,$(LEAVE_TEMPLATES) subdirs leave,$$(eval $$(call $(tpl)_leave_tpl)))
endef

#}}}
#{{{ hook top-level directory into loading

define dirs_config_tpl
  directory := .
  include $(SRCDIR)/Dir.mk
endef
CONFIG_TEMPLATES := dirs $(CONFIG_TEMPLATES)

#}}}
#{{{ create hook mechanism for per-module-and-dir config hooks

define process_module_directory
  $$(foreach tpl,$$(MODDIR_TEMPLATES),$$(eval $$(call $$(tpl)_moddir_tpl,$(1),$(2))))
endef
define dirs_module_tpl
  $$(foreach sp,$$(sp_list),$$(eval $$(call process_module_directory,$(1),$$(sp))))
endef
MOD_TEMPLATES := dirs $(MOD_TEMPLATES)

#}}}

endif
