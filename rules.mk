# Disable unused features
VIA_ENABLE = no
WPM_ENABLE = no
NKRO_ENABLE = no
AUDIO_ENABLE = no
MAGIC_ENABLE = no
STENO_ENABLE = no
LEADER_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
UNICODE_ENABLE = no
MOUSEKEY_ENABLE = no
TERMINAL_ENABLE = no
KEY_LOCK_ENABLE = no
RGBLIGHT_ENABLE = no
SLEEP_LED_ENABLE = no
TAP_DANCE_ENABLE = no
VELOCIKEY_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no

# Common features
WAIT_FOR_USB = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes

# Main source file and source paths
SRC += ykz89.c
VPATH += $(USER_PATH)/oled $(USER_PATH)/rgb $(USER_PATH)/features

# Custom features
# COMBO_ENABLE = yes
# SRC += combos.c
OPT_DEFS += -DCAPS_WORD
SRC += caps_word.c
# OPT_DEFS += -DHRM_AUDIT

ifeq ($(MCU), $(filter $(MCU), atmega32u4))
	LTO_ENABLE = yes
	BOOTLOADER = atmel-dfu
endif

# Corne CRKBD
ifeq ($(KEYBOARD), $(filter $(KEYBOARD), crkbd/rev1 keycapsss/kimiko))
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_DRIVER = WS2812
	SRC += rgb-matrix.c

	ifneq ($(strip $(OLED)),)
		OLED_ENABLE = yes
		OPT_DEFS += -D${OLED}
		ifeq ($(OLED), $(filter $(OLED), LEFTCAT RIGHTCAT CAT))
			SRC += oled-icons.c oled-bongocat.c
		else ifeq ($(OLED), $(filter $(OLED), LUNA FELIX))
			SRC += oled-icons.c oled-luna.c
		endif
	endif
endif