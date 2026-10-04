// Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over users/ykz89/: the Skeletyl thumb cluster (3 per hand), the
// full set, so no chords. No pointing device, so no pointer layer.

#include QMK_KEYBOARD_H
#include "ykz89.h"

#define LAYOUT_wrapper(...) LAYOUT_split_3x5_3(__VA_ARGS__)

// clang-format off
// Base thumb row; its innermost key per hand places QK_BOOT/EE_CLR (see ykz89.h).
#define THUMBS_BASE  ESC_MED, SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM, KC_DEL
#define THUMB_TUCK_L THUMB_PICK(2, THUMBS_BASE)
#define THUMB_TUCK_R THUMB_PICK(3, THUMBS_BASE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(
    LAYOUT_LAYER_BASE,      THUMBS_BASE)),
  [LAYER_FUNCTION]   = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION,   XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION, XXXXXXX, _______, XXXXXXX,  KC_ENT, KC_BSPC, _______),
  [LAYER_MEDIA]      = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA,      _______, KC_MPLY, KC_MSTP, KC_MSTP, KC_MPLY, KC_MUTE),
  [LAYER_NUMERAL]    = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL,     KC_DOT,    KC_0, KC_MINS, XXXXXXX, _______, XXXXXXX),
  [LAYER_SYMBOLS]    = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS,    KC_LPRN, KC_RPRN, KC_UNDS, _______, XXXXXXX, XXXXXXX),
};
// clang-format on
