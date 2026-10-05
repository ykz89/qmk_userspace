// Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
// Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over users/ykz89/: Dilemma trackball thumbs (3+3) and encoder map.
// Needs Bastardkb/bastardkb-qmk for the bk_pointing_device and argos modules
// (DPI_MOD, SNIPING, DRGSCRL, ...) and bk_led_matrix for the left-half LED matrix.

#include QMK_KEYBOARD_H
#include "ykz89.h"
#ifdef COMMUNITY_MODULE_BK_LED_MATRIX_ENABLE
#    include "led_matrix_layer_anims.h"
#    include "badapple.h"
#    include "doomfire.h"
#endif

#define LAYOUT_wrapper(...) LAYOUT_split_3x5_3(__VA_ARGS__)

#ifdef COMMUNITY_MODULE_BK_LED_MATRIX_ENABLE
// LED matrix animation key (LM_ANIM: next, Shift: previous) on the NUM layer,
// innermost key of the right top row (dead on this board: its boot keys are on SYM).
#    define NUM_WITH_ANIM_IMPL(L00, L01, L02, L03, L04, R05, ...) L00, L01, L02, L03, L04, LM_ANIM, __VA_ARGS__
#else
#    define NUM_WITH_ANIM_IMPL(...) __VA_ARGS__
#endif
#define NUM_WITH_ANIM(...) NUM_WITH_ANIM_IMPL(__VA_ARGS__)

// clang-format off
// Base thumb row; its innermost key per hand places QK_BOOT/EE_CLR (see ykz89.h).
#define THUMBS_BASE  ESC_MED, SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM, KC_DEL
#define THUMB_TUCK_L THUMB_PICK(2, THUMBS_BASE)
#define THUMB_TUCK_R THUMB_PICK(3, THUMBS_BASE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(POINTER_MOD(HOME_ROW_MOD_GACS(
    LAYOUT_LAYER_BASE,      THUMBS_BASE))),
  [LAYER_FUNCTION]   = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION,   XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION, XXXXXXX, _______, XXXXXXX,  KC_ENT, KC_BSPC, _______),
  [LAYER_MEDIA]      = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA,      _______, KC_MPLY, KC_MSTP, KC_MSTP, KC_MPLY, KC_MUTE),
  [LAYER_POINTER]    = LAYOUT_wrapper(LAYOUT_LAYER_POINTER,    KC_BTN2, KC_BTN1, KC_BTN3, KC_BTN3, KC_BTN1, KC_BTN2),
  [LAYER_NUMERAL]    = LAYOUT_wrapper(NUM_WITH_ANIM(LAYOUT_LAYER_NUMERAL),     KC_DOT,    KC_0, KC_MINS, XXXXXXX, _______, XXXXXXX),
  [LAYER_SYMBOLS]    = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS,    KC_LPRN, KC_RPRN, KC_UNDS, _______, XXXXXXX, XXXXXXX),
};
// clang-format on

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(KC_WH_D, KC_WH_U),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_FUNCTION]   = {ENCODER_CCW_CW(KC_DOWN, KC_UP),    ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
    [LAYER_NAVIGATION] = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP),  ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [LAYER_MEDIA]      = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP),  ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [LAYER_POINTER]    = {ENCODER_CCW_CW(RGB_HUD, RGB_HUI),  ENCODER_CCW_CW(RGB_SAD, RGB_SAI)},
    [LAYER_NUMERAL]    = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI),  ENCODER_CCW_CW(RGB_SPD, RGB_SPI)},
    [LAYER_SYMBOLS]    = {ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE

#ifdef COMMUNITY_MODULE_BK_LED_MATRIX_ENABLE
// Three-letter layer names for the left-half LED matrix (modules/bastardkb/bk_led_matrix).
const char *bklm_layer_name_user(uint8_t layer) {
    switch (layer) {
        case LAYER_BASE:       return "BSE";
        case LAYER_FUNCTION:   return "FUN";
        case LAYER_NAVIGATION: return "NAV";
        case LAYER_MEDIA:      return "MED";
        case LAYER_POINTER:    return "PTR";
        case LAYER_NUMERAL:    return "NUM";
        case LAYER_SYMBOLS:    return "SYM";
        default:               return NULL;
    }
}

// Animation shown while each layer is held (modules/bastardkb/bk_led_matrix/led_matrix_layer_anims.h).
uint8_t bklm_layer_anim_user(uint8_t layer) {
    switch (layer) {
        case LAYER_FUNCTION:   return BKLM_LAYER_ANIM_GEAR;
        case LAYER_NAVIGATION: return BKLM_LAYER_ANIM_NAVARROW;
        case LAYER_MEDIA:      return BKLM_LAYER_ANIM_EQUALIZER;
        case LAYER_POINTER:    return BKLM_LAYER_ANIM_HAND;
        case LAYER_NUMERAL:    return BKLM_LAYER_ANIM_DIGIT;
        case LAYER_SYMBOLS:    return BKLM_LAYER_ANIM_MATH;
        default:               return BKLM_LAYER_ANIM_NONE;
    }
}

// Custom trackball animations (LED_MATRIX_MODULE_MOTION_USER_COUNT in config.h).
bool bklm_motion_user(uint8_t index, RGB *pixels, const bklm_motion_t *m) {
    switch (index) {
        case 0:  return badapple_draw(pixels, m);
        default: return doomfire_draw(pixels, m);
    }
}
#endif // COMMUNITY_MODULE_BK_LED_MATRIX_ENABLE
