// Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over users/ykz89/: the Dilemma 3x5_2 thumb cluster (2 per hand).
// Each hand's tertiary thumb is a chord of the other two on every layer
// (ykz89_combos.c), so the rows are the 3-thumb rows minus the outer key.

#include QMK_KEYBOARD_H
#include "ykz89.h"

#define LAYOUT_wrapper(...) LAYOUT_split_3x5_2(__VA_ARGS__)

// clang-format off
// Base thumb row; its innermost key per hand places QK_BOOT/EE_CLR (see ykz89.h).
#define THUMBS_BASE  SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM
#define THUMB_TUCK_L THUMB_PICK(1, THUMBS_BASE)
#define THUMB_TUCK_R THUMB_PICK(2, THUMBS_BASE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(POINTER_MOD(HOME_ROW_MOD_GACS(
    LAYOUT_LAYER_BASE,      THUMBS_BASE))),
  [LAYER_FUNCTION]   = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION,   XXXXXXX, _______, XXXXXXX, XXXXXXX),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION, _______, XXXXXXX,  KC_ENT, KC_BSPC),
  [LAYER_MEDIA]      = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA,      KC_MPLY, KC_MSTP, KC_MSTP, KC_MPLY),
  [LAYER_POINTER]    = LAYOUT_wrapper(LAYOUT_LAYER_POINTER,    KC_BTN1, KC_BTN3, KC_BTN3, KC_BTN1),
  [LAYER_NUMERAL]    = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL,       KC_0, KC_MINS, XXXXXXX, _______),
  [LAYER_SYMBOLS]    = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS,    KC_RPRN, KC_UNDS, _______, XXXXXXX),
};
// clang-format on
