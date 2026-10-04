// Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
// Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over the shared keymap in users/ykz89/. The 4x6 has an extra top
// number row and extra outer columns; a wrapper wraps the shared 3x10 core with
// them. 8 thumbs (canonical 6 inner + 2 outer extras), so no combos.

#include QMK_KEYBOARD_H
#include "ykz89.h"

// clang-format off
// Wrap the shared 3x10 core with the 4x6's number row, outer columns + thumbs.
//   core (30) , number row (12) , outer L per row (3) , outer R per row (3) , thumbs (8)
#define _D4X6_WRAP(                                                                  \
    l00, l01, l02, l03, l04, r05, r06, r07, r08, r09,                               \
    l10, l11, l12, l13, l14, r15, r16, r17, r18, r19,                               \
    l20, l21, l22, l23, l24, r25, r26, r27, r28, r29,                               \
    n00, n01, n02, n03, n04, n05, n06, n07, n08, n09, n10, n11,                      \
    OL0, OL1, OL2, OR0, OR1, OR2,                                                    \
    t0, t1, t2, t3, t4, t5, t6, t7)                                                 \
    n00, n01, n02, n03, n04, n05,   n06, n07, n08, n09, n10, n11,                    \
    OL0, l00, l01, l02, l03, l04,   r05, r06, r07, r08, r09, OR0,                    \
    OL1, l10, l11, l12, l13, l14,   r15, r16, r17, r18, r19, OR1,                    \
    OL2, l20, l21, l22, l23, l24,   r25, r26, r27, r28, r29, OR2,                    \
                   t0, t1, t2, t3,   t4, t5, t6, t7
#define D4X6_WRAP(...) _D4X6_WRAP(__VA_ARGS__)
#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

#define ____NUM_ROW____    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS
#define ____NUM_ROW_T____  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______

// Base thumb row; its innermost key per hand places QK_BOOT/EE_CLR (see ykz89.h).
#define THUMBS_BASE  KC_LALT, ESC_MED, SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM, KC_DEL, KC_MUTE
#define THUMB_TUCK_L THUMB_PICK(3, THUMBS_BASE)
#define THUMB_TUCK_R THUMB_PICK(4, THUMBS_BASE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(D4X6_WRAP(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE)),
    ____NUM_ROW____,
    KC_TAB, KC_LSFT, KC_LCTL,   KC_BSLS, KC_QUOT, KC_LALT,
    THUMBS_BASE)),

  [LAYER_FUNCTION] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_FUNCTION,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, _______,   XXXXXXX, XXXXXXX, _______, _______)),

  [LAYER_NAVIGATION] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_NAVIGATION,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______, XXXXXXX, _______, XXXXXXX,    KC_ENT, KC_BSPC, _______, _______)),

  [LAYER_MEDIA] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_MEDIA,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______, _______, KC_MPLY, KC_MSTP,   KC_MSTP, KC_MPLY, KC_MUTE, _______)),

  [LAYER_POINTER] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_POINTER,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______, KC_BTN2, KC_BTN1, KC_BTN3,   KC_BTN3, KC_BTN1, KC_BTN2, _______)),

  [LAYER_NUMERAL] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_NUMERAL,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______,  KC_DOT,    KC_0, KC_MINS,   XXXXXXX, _______, XXXXXXX, _______)),

  [LAYER_SYMBOLS] = LAYOUT_wrapper(D4X6_WRAP(
    LAYOUT_LAYER_SYMBOLS,
    ____NUM_ROW_T____,
    _______, _______, _______,   _______, _______, _______,
    _______, KC_LPRN, KC_RPRN, KC_UNDS,   _______, XXXXXXX, XXXXXXX, _______)),
};
// clang-format on

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_FUNCTION]   = {ENCODER_CCW_CW(KC_UP, KC_DOWN),    ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
    [LAYER_NAVIGATION] = {ENCODER_CCW_CW(KC_PGUP, KC_PGDN),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_MEDIA]      = {ENCODER_CCW_CW(KC_MPRV, KC_MNXT),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_POINTER]    = {ENCODER_CCW_CW(RGB_HUD, RGB_HUI),  ENCODER_CCW_CW(RGB_SAD, RGB_SAI)},
    [LAYER_NUMERAL]    = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI),  ENCODER_CCW_CW(RGB_SPD, RGB_SPI)},
    [LAYER_SYMBOLS]    = {ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE
