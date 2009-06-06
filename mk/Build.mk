#!/usr/bin/make -f

$(if $(MK_INCLUDE),,$(error This makefile is meant for inclusion by other makefiles))

ADDONDIR:=$(srcdir)/mk

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

.PHONY: default all clean
clean:
	$(if $(CLEAN),rm -f $(CLEAN) || true,@:)

