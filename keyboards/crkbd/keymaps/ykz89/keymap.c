// Copyright 2022 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over users/ykz89/. A wrapper adds the Corne's outer pinky
// columns around the shared 3x10 core; thumbs are the full set, so no chords.
// No pointing device, so no pointer layer. OLED rendering is board-specific.

#include QMK_KEYBOARD_H
#include "ykz89.h"

// clang-format off
// core (30), outer L (3), outer R (3), thumbs (6) -> LAYOUT_split_3x6_3
#define _CRKBD_WRAP(                                              \
    l00, l01, l02, l03, l04, r05, r06, r07, r08, r09,            \
    l10, l11, l12, l13, l14, r15, r16, r17, r18, r19,            \
    l20, l21, l22, l23, l24, r25, r26, r27, r28, r29,            \
    OL0, OL1, OL2, OR0, OR1, OR2,                                \
    t0, t1, t2, t3, t4, t5)                                      \
    OL0, l00, l01, l02, l03, l04,   r05, r06, r07, r08, r09, OR0, \
    OL1, l10, l11, l12, l13, l14,   r15, r16, r17, r18, r19, OR1, \
    OL2, l20, l21, l22, l23, l24,   r25, r26, r27, r28, r29, OR2, \
                        t0, t1, t2, t3, t4, t5
#define CRKBD_WRAP(...) _CRKBD_WRAP(__VA_ARGS__)
#define LAYOUT_wrapper(...) LAYOUT_split_3x6_3(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(CRKBD_WRAP(
    HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE),
    KC_TAB, KC_LSFT, KC_LCTL,   KC_MINS, KC_SCLN, KC_LSFT,
    ESC_MED, SPC_NAV, TAB_FUN,  ENT_SYM, BSP_NUM, KC_DEL)),

  [LAYER_FUNCTION] = LAYOUT_wrapper(CRKBD_WRAP(
    LAYOUT_LAYER_FUNCTION,
    _______, _______, _______,  _______, _______, _______,
    XXXXXXX, XXXXXXX, _______,  XXXXXXX, XXXXXXX, _______)),

  [LAYER_NAVIGATION] = LAYOUT_wrapper(CRKBD_WRAP(
    LAYOUT_LAYER_NAVIGATION,
    _______, _______, _______,  _______, _______, _______,
    XXXXXXX, _______, XXXXXXX,   KC_ENT, KC_BSPC, _______)),

  [LAYER_MEDIA] = LAYOUT_wrapper(CRKBD_WRAP(
    LAYOUT_LAYER_MEDIA,
    _______, _______, _______,  _______, _______, _______,
    _______, KC_MPLY, KC_MSTP,  KC_MSTP, KC_MPLY, KC_MUTE)),

  [LAYER_NUMERAL] = LAYOUT_wrapper(CRKBD_WRAP(
    LAYOUT_LAYER_NUMERAL,
    _______, _______, _______,  _______, _______, _______,
     KC_DOT,    KC_0, KC_MINS,  XXXXXXX, _______, XXXXXXX)),

  [LAYER_SYMBOLS] = LAYOUT_wrapper(CRKBD_WRAP(
    LAYOUT_LAYER_SYMBOLS,
    _______, _______, _______,  _______, _______, _______,
    KC_LPRN, KC_RPRN, KC_UNDS,  _______, XXXXXXX, XXXXXXX)),
};
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void render_layer_state(void) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case LAYER_BASE:
            oled_write_ln_P(PSTR("Base \n"), false);
            break;
        case LAYER_NAVIGATION:
            oled_write_ln_P(PSTR("Nav  \n"), false);
            break;
        case LAYER_MEDIA:
            oled_write_ln_P(PSTR("Media\n"), false);
            break;
        case LAYER_NUMERAL:
            oled_write_ln_P(PSTR("Num  \n"), false);
            break;
        case LAYER_FUNCTION:
            oled_write_ln_P(PSTR("Fun  \n"), false);
            break;
        case LAYER_SYMBOLS:
            oled_write_ln_P(PSTR("Sym  \n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("     \n"), false);
            break;
    }
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

void render_logo(void) {
    static const char PROGMEM corne_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(corne_logo, false);
    oled_write_P(PSTR("corne"), false);
}

void render_state(void) {
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
}

bool oled_task_user(void) {
    render_space();
    render_logo();
    render_space();
    render_space();
    render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
    render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
    render_space();
    render_layer_state();
    render_space();
    render_state();
    return false;
}
#endif

#ifdef RGB_MATRIX_ENABLE

void suspend_power_down_keymap(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_keymap(void) {
    rgb_matrix_set_suspend_state(false);
}

#endif
