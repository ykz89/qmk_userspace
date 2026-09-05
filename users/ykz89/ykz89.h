// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

/**
 * Shared keymap for all of @ykz89's boards: Colemak-DH with Miryoku-style
 * layers. The 3x10 core of every layer lives here; each board's keymap.c adds
 * its thumb cluster and any extra keys.
 *
 * Thumb keys per hand, in Miryoku's column order, outermost first:
 *   tertiary   ESC_MED / KC_DEL    outer
 *   primary    SPC_NAV / BSP_NUM   middle
 *   secondary  TAB_FUN / ENT_SYM   inner
 * Boards with two thumbs per hand drop the tertiary and get it back as a chord
 * of the other two on every layer (COMBO_THUMB_L/R, see ykz89_combos.c); the
 * primary is then the outer of the two.
 *
 * le_chiffre is the one deliberate exception. Its 2u thumb key sits inboard
 * and is where the thumb rests, so there the primary takes the 2u key and the
 * order is reversed: TAB SPACE | BSPC ENT read left to right. See
 * keyboards/sporkus/le_chiffre_stm32/keymaps/ykz89/keymap.c.
 */

// The pointer layer exists only with a pointing device; without one LT_PTR and
// POINTER_MOD are passthroughs.
enum ykz89_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_MEDIA,
#ifdef POINTING_DEVICE_ENABLE
    LAYER_POINTER,
#endif
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
};

// Layer-tap thumb keys.
#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_FUN LT(LAYER_FUNCTION, KC_TAB)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#ifdef POINTING_DEVICE_ENABLE
#    define LT_PTR(KC) LT(LAYER_POINTER, KC)
#else
#    define LT_PTR(KC) KC
#endif

// Aliases for keycodes QMK has renamed, so the layers below read unchanged.
// Older trees (vial-qmk) still define some as legacy aliases; drop those first.
#undef KC_BTN1
#undef KC_BTN2
#undef KC_BTN3
#undef KC_WH_U
#undef KC_WH_D
#undef RGB_TOG
#undef RGB_MOD
#undef RGB_RMOD
#undef RGB_HUI
#undef RGB_HUD
#undef RGB_SAI
#undef RGB_SAD
#undef RGB_VAI
#undef RGB_VAD
#undef RGB_SPI
#undef RGB_SPD

// Mouse buttons + wheel.
#define KC_BTN1 MS_BTN1
#define KC_BTN2 MS_BTN2
#define KC_BTN3 MS_BTN3
#define KC_WH_U MS_WHLU
#define KC_WH_D MS_WHLD

// RGB: resolve to RGB Matrix (RM_*), RGBLight (UG_*) or nothing, so the Media
// layer and encoder maps stay board-agnostic.
#if defined(RGB_MATRIX_ENABLE)
#    define RGB_TOG  RM_TOGG
#    define RGB_MOD  RM_NEXT
#    define RGB_RMOD RM_PREV
#    define RGB_HUI  RM_HUEU
#    define RGB_HUD  RM_HUED
#    define RGB_SAI  RM_SATU
#    define RGB_SAD  RM_SATD
#    define RGB_VAI  RM_VALU
#    define RGB_VAD  RM_VALD
#    define RGB_SPI  RM_SPDU
#    define RGB_SPD  RM_SPDD
#elif defined(RGBLIGHT_ENABLE)
#    define RGB_TOG  UG_TOGG
#    define RGB_MOD  UG_NEXT
#    define RGB_RMOD UG_PREV
#    define RGB_HUI  UG_HUEU
#    define RGB_HUD  UG_HUED
#    define RGB_SAI  UG_SATU
#    define RGB_SAD  UG_SATD
#    define RGB_VAI  UG_VALU
#    define RGB_VAD  UG_VALD
#    define RGB_SPI  UG_SPDU
#    define RGB_SPD  UG_SPDD
#else
#    define RGB_TOG  XXXXXXX
#    define RGB_MOD  XXXXXXX
#    define RGB_RMOD XXXXXXX
#    define RGB_HUI  XXXXXXX
#    define RGB_HUD  XXXXXXX
#    define RGB_SAI  XXXXXXX
#    define RGB_SAD  XXXXXXX
#    define RGB_VAI  XXXXXXX
#    define RGB_VAD  XXXXXXX
#    define RGB_SPI  XXXXXXX
#    define RGB_SPD  XXXXXXX
#endif

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

/*
 * Layer cores (3 rows x 10 columns, no thumbs). These are identical on every
 * board. Thumbs are appended per-board in each keymap.c.
 *
 * Started from the Miryoku layout, trimmed and personalized.
 * See https://github.com/manna-harbour/miryoku for the original.
 */

/** \brief Colemak-DH base. RALT (AltGr) lives on X and `.`; with a pointing device, POINTER_MOD also puts a pointer-layer LT on Z and `/`. */
#define LAYOUT_LAYER_BASE                                                                              \
       KC_Q,         KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,         KC_Y, KC_QUOT, \
       KC_A,         KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,         KC_I, KC_O,    \
       KC_Z, RALT_T(KC_X),    KC_C,    KC_D,    KC_V,    KC_K,    KC_H, KC_COMM, RALT_T(KC_DOT), KC_SLSH

/** \brief Function layer: F-keys + system keys mirroring the numeral layout; bootloader + EEPROM clear on the top-left keys. */
#define LAYOUT_LAYER_FUNCTION                                                                 \
    QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12, \
    ______________HOME_ROW_GACS_L______________, KC_SCRL,   KC_F4,   KC_F5,   KC_F6,  KC_F11, \
    _______________DEAD_HALF_ROW_______________, KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10

/** \brief Navigation layer: arrows on the home position, caps-word on the inner column. */
#define LAYOUT_LAYER_NAVIGATION                                                               \
    _______________DEAD_HALF_ROW_______________, _______________DEAD_HALF_ROW_______________, \
    ______________HOME_ROW_GACS_L______________, CW_TOGG, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, \
    _______________DEAD_HALF_ROW_______________,  KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END

/**
 * \brief Media + RGB layer (symmetrical for left/right trackball).
 *
 * Per hand: toggle and next-mode on the pinky column (Shift + mode steps back),
 * then hue, saturation, brightness and speed, up on the top row and down on
 * the bottom row. Media transport sits on the home row.
 */
#define LAYOUT_LAYER_MEDIA                                                                    \
    RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG, \
    KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, \
    RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, RGB_MOD

/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                  \
    XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______

/** \brief Numerals in numpad positions with symbols around them. */
#define LAYOUT_LAYER_NUMERAL                                                                  \
    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, _______________DEAD_HALF_ROW_______________, \
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, ______________HOME_ROW_GACS_R______________, \
     KC_GRV,    KC_1,    KC_2,    KC_3, KC_BSLS, _______________DEAD_HALF_ROW_______________

/** \brief Shifted symbols mirroring the numeral positions; bootloader + EEPROM clear on the top-right keys. */
#define LAYOUT_LAYER_SYMBOLS                                                                  \
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR, QK_BOOT, \
    KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, ______________HOME_ROW_GACS_R______________, \
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE, _______________DEAD_HALF_ROW_______________

/**
 * \brief Add GACS (Gui, Alt, Ctl, Shift) home-row mods to a 10-key-per-row layout.
 *
 * Both hands send the LEFT modifier keycodes, so the two home rows mirror each
 * other exactly. RALT (AltGr) on the base layer's X and `.` is unaffected: it
 * is a distinct keycode, not a handedness choice.
 *
 * The first 20 keycodes are the top two rows; everything after (bottom row +
 * thumbs) flows through unchanged via __VA_ARGS__.
 */
#define HOME_ROW_MOD_GACS_IMPL(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),        L14,  \
             R15,  LSFT_T(R16), LCTL_T(R17), LALT_T(R18), LGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) HOME_ROW_MOD_GACS_IMPL(__VA_ARGS__)

/**
 * \brief Add pointer-layer access (LT on the bottom outer keys) to a layout.
 *
 * Consumes 30 keycodes (3 rows); thumbs flow through via __VA_ARGS__.
 */
#define POINTER_MOD_IMPL(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      LT_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  LT_PTR(R29)  \
      , ##__VA_ARGS__
#define POINTER_MOD(...) POINTER_MOD_IMPL(__VA_ARGS__)
// clang-format on
