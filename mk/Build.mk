#!/usr/bin/make -f

default: all

MK_INCLUDE:=1
SRCDIR:=.
BUILDDIR:=.
DEPDIR:=.deps
ADDONDIR:=$(SRCDIR)/mk

include $(BUILDDIR)/Config.mk
$(foreach MK,$(ADDON_MK),$(eval include $(ADDONDIR)/$(MK).mk))

define config_top_tpl
  $$(foreach tpl,$$(CONFIG_TEMPLATES),$$(eval $$(call $$(tpl)_config_tpl)))
endef
define post_config_top_tpl
  $$(foreach tpl,$$(POSTCONF_TEMPLATES),$$(eval $$(call $$(tpl)_postconfig_tpl)))
endef
define modules_top_tpl
  $$(foreach mod,$(MODULES),$$(foreach tpl,$$(MOD_TEMPLATES),$$(eval $$(call $$(tpl)_module_tpl,$$(mod)))))
endef
define post_modules_top_tpl
  $$(foreach tpl,$$(POSTMOD_TEMPLATES),$$(eval $$(call $$(tpl)_postmod_tpl)))
endef
define rules_top_tpl
  $$(foreach tpl,$$(RULES_TEMPLATES),$$(eval $$(call $$(tpl)_rules_tpl)))
endef
TOP_TEMPLATES := config post_config modules post_modules rules
$(foreach tpl,$(TOP_TEMPLATES),$(eval $(call $(tpl)_top_tpl)))

CLEAN:=$(CLEAN) $(OBJECTS) $(BINARIES) $(DEPENDS)

.PHONY: default all clean
clean:
	rm -f $(CLEAN) || true

