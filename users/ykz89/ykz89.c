// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "ykz89.h"

// Combos live in ykz89_combos.c, pulled in via INTROSPECTION_KEYMAP_C so QMK's
// keymap introspection can size the `key_combos` array. See users/ykz89/rules.mk.

/* -------------------------------------------------------------------------- *
 * Pointing device: auto pointer layer + auto sniping.
 * Both are opt-in per board (config.h), mirroring the original per-keymap code.
 * -------------------------------------------------------------------------- */
#ifdef POINTING_DEVICE_ENABLE

#    ifdef AUTO_POINTER
#        include "timer.h"

#        ifndef AUTO_POINTER_TIMEOUT_MS
#            define AUTO_POINTER_TIMEOUT_MS 1000
#        endif
#        ifndef AUTO_POINTER_THRESHOLD
#            define AUTO_POINTER_THRESHOLD 8
#        endif

static uint16_t auto_pointer_layer_timer = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > AUTO_POINTER_THRESHOLD || abs(mouse_report.y) > AUTO_POINTER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= AUTO_POINTER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // AUTO_POINTER

#    ifdef AUTO_SNIPING
layer_state_t layer_state_set_user(layer_state_t state) {
#        if defined(KEYBOARD_bastardkb_charybdis)
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, LAYER_POINTER));
#        elif defined(KEYBOARD_bastardkb_dilemma)
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, LAYER_POINTER));
#        endif
    return state;
}
#    endif // AUTO_SNIPING

#endif // POINTING_DEVICE_ENABLE
