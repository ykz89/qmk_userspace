// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Values below follow BastardKB's vendor keymap for this board.

#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

#define ENCODER_RESOLUTION 4

// Auto-mouse (toggled from Argos) jumps to the pointer layer; LAYER_POINTER
// is 4 in users/ykz89/ykz89.h.
#ifdef AUTO_MOUSE_DEFAULT_LAYER
#    undef AUTO_MOUSE_DEFAULT_LAYER
#endif
#define AUTO_MOUSE_DEFAULT_LAYER 4

#ifdef LED_DPI_INDICATOR_INDEX
#    undef LED_DPI_INDICATOR_INDEX
#endif
#define LED_DPI_INDICATOR_INDEX 7

// Argos sizes its per-key RGB storage from this.
#ifdef RGBLIGHT_LED_COUNT
#    undef RGBLIGHT_LED_COUNT
#endif
#define RGBLIGHT_LED_COUNT 72

// Left-half 12x16 WS2812 LED matrix (modules/ykz89/bk_led_matrix), on the VIK port.
#define LED_MATRIX_MODULE_PIN GP12
