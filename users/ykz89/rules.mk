# Shared sources and features for all of @ykz89's keymaps.

# ykz89.c is entirely #ifdef POINTING_DEVICE_ENABLE (resolved before this file).
ifeq ($(strip $(POINTING_DEVICE_ENABLE)), yes)
SRC += ykz89.c
endif

# Combos go through keymap introspection, not SRC, so ARRAY_SIZE(key_combos)
# works. Vial supplies its own key_combos[], so skip them there.
# COMBO_SHOULD_TRIGGER pins each chord to its layer (see ykz89_combos.c).
ifneq ($(strip $(VIAL_ENABLE)), yes)
INTROSPECTION_KEYMAP_C += ykz89_combos.c
OPT_DEFS += -DCOMBO_SHOULD_TRIGGER
endif

# CW_TOGG sits on the navigation layer.
CAPS_WORD_ENABLE = yes
