/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/* Disable unused features. */
#define NO_ACTION_ONESHOT

#define POINTING_DEVICE_RIGHT
#define POINTING_DEVICE_INVERT_X

#ifndef TAPPING_TERM
/**
 * \brief Configure the global tapping term (default: 200ms).
 *
 * If you have a lot of accidental mod activations, crank up the tapping term.
 *
 * See docs.qmk.fm/using-qmk/software-features/tap_hold#tapping-term
 */
#    define TAPPING_TERM 200
#endif  // TAPPING_TERM

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Enable pointer acceleration, which increases the speed by ~2x for large
// displacement, while maintaining 1x speed for slow movements.
// - `CHARYBDIS_POINTER_ACCELERATION_FACTOR`
#    define CHARYBDIS_POINTER_ACCELERATION_ENABLE

// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
//    #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif  // POINTING_DEVICE_ENABLE

#define RETRO_TAPPING

// Prevent normal rollover on alphas from accidentally triggering mods.
#define IGNORE_MOD_TAP_INTERRUPT

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD

// Auto Shift
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4

#define COMBO_COUNT 8
#define COMBO_TERM 200
#define EXTRA_SHORT_COMBOS

#ifdef OLED_ENABLE
#   define SPLIT_OLED_ENABLE
#   define SPLIT_MODS_ENABLE
#   define SPLIT_LED_STATE_ENABLE
#   define SPLIT_LAYER_STATE_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_UPDATE_INTERVAL 50
#	define OLED_BRIGHTNESS 180
#	define OLED_FONT_H "oled/oledfont.c"
#endif

#ifdef KEYBOARD_keycapsss_kimiko
    #ifdef RGB_MATRIX_ENABLE
    #    define DRIVER_LED_TOTAL RGBLED_NUM
    #    define RGBLED_NUM 60  // Number of LEDs
    #    define RGB_MATRIX_SPLIT { 30, 30 }  	// (Optional) For split keyboards, the number of LEDs connected on each half. X = left, Y = Right.
    #    define SPLIT_TRANSPORT_MIRROR  // If RGB_MATRIX_KEYPRESSES or RGB_MATRIX_KEYRELEASES is enabled, you also will want to enable SPLIT_TRANSPORT_MIRROR
    #endif
#endif

#include "keymaps/layout.h"