#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))

ifndef DIST_MK_INCLUDED
DIST_MK_INCLUDED := 1

include $(ADDONDIR)/Dirs.mk

install = install
install_dir = $(install) -d
install_data = $(install) -c -m 644
install_doc = $(install_data)
install_program = $(install) -c

define distfiles_leave_tpl
  DOCS:=$$(DOCS)$$(if $$(DOCS_$$(sp)), $$(DOCS_$$(sp)),)
  DATA:=$$(DATA)$$(if $$(DATA_$$(sp)), $$(DATA_$$(sp)),)
  $$(if $$(DATA_$$(sp)),$$(eval DATA_DIRS:=$$(DATA_DIRS) $$(d)))
  NOINST:=$$(NOINST)$$(if $$(NOINST_$$(sp)), $$(NOINST_$$(sp)),)
endef
LEAVE_TEMPLATES := $(LEAVE_TEMPLATES) distfiles

define install_doc_rule
  .PHONY: $$(DESTDIR)$$(docdir)/$$(notdir $(1))
  $$(DESTDIR)$$(docdir)/$$(notdir $(1)): $(1) install-doc-dirs
	$$(install_doc) $$< $$@
endef
define install_data_rule
  .PHONY: $$(DESTDIR)$$(pkgdatadir)/$(1)
  $$(DESTDIR)$$(pkgdatadir)/$(1): $(1) install-data-dirs
	$$(install_data) $$< $$@
endef
define install_prog_rule
  .PHONY: $$(DESTDIR)$$(bindir)/$(1)
  $$(DESTDIR)$$(bindir)/$(1): $(1) install-prog-dirs
	$$(install_program) $$< $$@
endef
define install_rules_tpl
  .PHONY: install-doc-dirs install-data-dirs install-prog-dirs install-dirs install-docs install-data install-progs install
  install-doc-dirs:
	$$(install_dir) $$(DESTDIR)$$(docdir)
  install-docs: $$(foreach doc,$$(DOCS),$$(DESTDIR)$$(docdir)/$$(notdir $$(doc)))
  $$(foreach doc,$$(DOCS),$$(eval $$(call install_doc_rule,$$(doc))))
  install-data-dirs:
	$$(install_dir) $$(addprefix $$(DESTDIR)$$(pkgdatadir)/,$$(DATA_DIRS))
  install-data: $$(foreach data,$$(DATA),$$(DESTDIR)$$(pkgdatadir)/$$(data))
  $$(foreach data,$$(DATA),$$(eval $$(call install_data_rule,$$(data))))
  install-prog-dirs:
	$$(install_dir) $$(DESTDIR)$$(bindir)
  install-progs: $$(foreach bin,$$(BINARIES),$$(DESTDIR)$$(bindir)/$$(notdir $$(bin)))
  $$(foreach bin,$$(BINARIES),$$(eval $$(call install_prog_rule,$$(bin))))
  install: install-progs install-data install-docs
endef
RULES_TEMPLATES := $(RULES_TEMPLATES) install

endif
