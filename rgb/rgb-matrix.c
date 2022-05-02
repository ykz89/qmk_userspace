// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"

void rgb_matrix_indicators_user(void) {
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Modifier keys indicator
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) > BSE) {
		uint8_t layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				if (g_led_config.matrix_co[row][col] != NO_LED &&
				keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	}
}