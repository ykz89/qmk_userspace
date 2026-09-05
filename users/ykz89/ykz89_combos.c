// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thumb chords for boards with only two thumbs per hand: the tertiary thumb
// becomes a chord of the primary and secondary on every layer (Miryoku's 34-key
// rule). Included via INTROSPECTION_KEYMAP_C, not SRC, so QMK can size
// key_combos[]; see users/ykz89/rules.mk.
//
// QMK matches combos on keycodes, so every layer needs its own chord, using the
// keycodes the boards put on those thumbs (the Skeletyl rows minus the outer
// key). The other hand holds the layer, so the chord is on the free hand:
//
//   layer    chord                  gives
//   base     SPC_NAV + TAB_FUN  ->  ESC_MED
//   base     ENT_SYM + BSP_NUM  ->  KC_DEL
//   NAV      KC_BSPC + KC_ENT   ->  KC_DEL
//   MEDIA    KC_MPLY + KC_MSTP  ->  KC_MUTE
//   NUM      KC_0    + KC_MINS  ->  KC_DOT
//   SYM      KC_RPRN + KC_UNDS  ->  KC_LPRN
//   POINTER  KC_BTN1 + KC_BTN3  ->  KC_BTN2   (either hand)
//
// Opt in per hand in the keymap's config.h: COMBO_THUMB_L (base, NUM, SYM)
// and/or COMBO_THUMB_R (base, NAV, MEDIA). The POINTER chord comes with either
// on boards with a pointing device. Needs COMBO_ENABLE in rules.mk.

#include QMK_KEYBOARD_H
#include "ykz89.h"

#ifdef COMBO_ENABLE

#    if !defined(COMBO_THUMB_L) && !defined(COMBO_THUMB_R)
#        error "COMBO_ENABLE is set but no thumb side opted in; define COMBO_THUMB_L and/or COMBO_THUMB_R in the keymap's config.h, or drop COMBO_ENABLE."
#    endif

#    if defined(POINTING_DEVICE_ENABLE)
#        define COMBO_THUMB_PTR
#    endif

enum ykz89_combos {
#    ifdef COMBO_THUMB_L
    CMB_L_BASE,
    CMB_L_NUM,
    CMB_L_SYM,
#    endif
#    ifdef COMBO_THUMB_R
    CMB_R_BASE,
    CMB_R_NAV,
    CMB_R_MEDIA,
#    endif
#    ifdef COMBO_THUMB_PTR
    CMB_PTR,
#    endif
    COMBO_LENGTH // last
};

#    ifdef COMBO_THUMB_L
const uint16_t PROGMEM combo_l_base[] = {SPC_NAV, TAB_FUN, COMBO_END};
const uint16_t PROGMEM combo_l_num[]  = {KC_0, KC_MINS, COMBO_END};
const uint16_t PROGMEM combo_l_sym[]  = {KC_RPRN, KC_UNDS, COMBO_END};
#    endif
#    ifdef COMBO_THUMB_R
const uint16_t PROGMEM combo_r_base[]  = {ENT_SYM, BSP_NUM, COMBO_END};
const uint16_t PROGMEM combo_r_nav[]   = {KC_BSPC, KC_ENT, COMBO_END};
const uint16_t PROGMEM combo_r_media[] = {KC_MPLY, KC_MSTP, COMBO_END};
#    endif
#    ifdef COMBO_THUMB_PTR
const uint16_t PROGMEM combo_ptr[] = {KC_BTN1, KC_BTN3, COMBO_END};
#    endif

combo_t key_combos[] = {
#    ifdef COMBO_THUMB_L
    [CMB_L_BASE] = COMBO(combo_l_base, ESC_MED),
    [CMB_L_NUM]  = COMBO(combo_l_num, KC_DOT),
    [CMB_L_SYM]  = COMBO(combo_l_sym, KC_LPRN),
#    endif
#    ifdef COMBO_THUMB_R
    [CMB_R_BASE]  = COMBO(combo_r_base, KC_DEL),
    [CMB_R_NAV]   = COMBO(combo_r_nav, KC_DEL),
    [CMB_R_MEDIA] = COMBO(combo_r_media, KC_MUTE),
#    endif
#    ifdef COMBO_THUMB_PTR
    [CMB_PTR] = COMBO(combo_ptr, KC_BTN2),
#    endif
};

#    ifdef COMBO_SHOULD_TRIGGER
/** Pin each chord to its layer, so a number row rolling 0 and - on the base layer never fires the NUM chord. */
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    const uint8_t layer = get_highest_layer(layer_state | default_layer_state);

    switch (combo_index) {
#        ifdef COMBO_THUMB_L
        case CMB_L_BASE:
            return layer == LAYER_BASE;
        case CMB_L_NUM:
            return layer == LAYER_NUMERAL;
        case CMB_L_SYM:
            return layer == LAYER_SYMBOLS;
#        endif
#        ifdef COMBO_THUMB_R
        case CMB_R_BASE:
            return layer == LAYER_BASE;
        case CMB_R_NAV:
            return layer == LAYER_NAVIGATION;
        case CMB_R_MEDIA:
            return layer == LAYER_MEDIA;
#        endif
#        ifdef COMBO_THUMB_PTR
        case CMB_PTR:
            return layer == LAYER_POINTER;
#        endif
        default:
            return true;
    }
}
#    endif // COMBO_SHOULD_TRIGGER

#endif // COMBO_ENABLE
