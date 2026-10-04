// Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
// Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over the shared keymap in users/ykz89/. Supplies the Procyon
// thumb cluster (6 keys: 3 left, 3 right) and keeps the board-specific encoder
// map + macOS-trackpad/digitizer plumbing.

#include QMK_KEYBOARD_H
#include "ykz89.h"
#include "digitizer.h"
#include "host.h"

static uint8_t digitizer_button_state = 0;

#define LAYOUT_wrapper(...) LAYOUT_split_3x5_3(__VA_ARGS__)

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
  [LAYER_NUMERAL]    = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL,     KC_DOT,    KC_0, KC_MINS, XXXXXXX, _______, XXXXXXX),
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

#ifdef MACOS_TRACKPAD_MODE
#include "pointing_device.h"
#include "timer.h"

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    mouse_report = pointing_device_task_user(mouse_report);

#if defined(DIGITIZER_ENABLE) && defined(DIGITIZER_FINGER_COUNT) && defined(DIGITIZER_CONTACT_COUNT)
    static uint32_t scan_time = 0;
    static int last_contacts = 0;
    static uint32_t inactivity_timer = 0;

    bool has_mouse_movement = (mouse_report.x != 0 || mouse_report.y != 0);

    digitizer_t digitizer_state = digitizer_get_state();
    bool has_buttons = (digitizer_state.button1 || digitizer_state.button2 || digitizer_state.button3);

    bool has_contacts = false;
    const int max_contacts = (DIGITIZER_FINGER_COUNT < DIGITIZER_CONTACT_COUNT) ? DIGITIZER_FINGER_COUNT : DIGITIZER_CONTACT_COUNT;
    for (int i = 0; i < max_contacts && i < DIGITIZER_CONTACT_COUNT; i++) {
        if (digitizer_state.contacts[i].type == FINGER && digitizer_state.contacts[i].tip) {
            has_contacts = true;
            break;
        }
    }

    if (!has_contacts && !has_buttons && !has_mouse_movement) {
        inactivity_timer = timer_read32();
        last_contacts = 0;
        scan_time = 0; // Reset scan_time on inactivity so it's recalculated when activity resumes
        return mouse_report;
    }

    // Digitizer report with button state from physical keys mapped to mouse buttons
    // macOS uses the Digitizer interface for trackpad pointing, so it expects button presses
    // to come from the same interface. We send buttons to BOTH interfaces:
    // - Mouse interface: For Linux/other OSes that use the Mouse interface
    // - Digitizer interface: For macOS which uses the Digitizer interface
    // Note: Only keys mapped to QK_MOUSE_BUTTON_1/2/3 are sent as digitizer buttons.
    // Other keys (KC_MUTE, DPI_INC, etc.) are sent through the keyboard HID interface.
    // Finger tap gestures are handled separately by the core digitizer gesture system.
    report_digitizer_t digitizer_report = {
        .report_id = REPORT_ID_DIGITIZER,
        .scan_time = 0,
        .contact_count = 0,
        // .button1 = digitizer_state.button1, // Key mapped to QK_MOUSE_BUTTON_1 (left click)
        // .button2 = digitizer_state.button2, // Key mapped to QK_MOUSE_BUTTON_2 (right click)
        // .button3 = digitizer_state.button3, // Key mapped to QK_MOUSE_BUTTON_3 (middle click)
        .reserved2 = 0
    };

    int contacts = 0;

    if (has_contacts) {
        for (int i = 0; i < max_contacts && i < DIGITIZER_CONTACT_COUNT; i++) {
            if (digitizer_state.contacts[i].type == FINGER && digitizer_state.contacts[i].tip) {
                contacts++;

                if (digitizer_report.contact_count < DIGITIZER_FINGER_COUNT) {
                    digitizer_report.fingers[digitizer_report.contact_count] = (digitizer_finger_report_t){
                        .confidence = digitizer_state.contacts[i].confidence,
                        .tip = 1,
                        .reserved = 0,
                        .contact_id = i,
                        .reserved2 = 0,
                        .x = digitizer_state.contacts[i].x,
                        .y = digitizer_state.contacts[i].y
                    };
                    digitizer_report.contact_count++;
                }
            }
        }
    }

    // Initialize or reset scan_time when contacts first appear after inactivity
    if (scan_time == 0 && contacts > 0) {
        scan_time = timer_read32();
    } else if (last_contacts == 0 && contacts > 0 && timer_elapsed32(inactivity_timer) > DIGITIZER_INACTIVITY_TIMEOUT_MS) {
        scan_time = timer_read32();
    }
    inactivity_timer = timer_read32();
    last_contacts = contacts;

    // Calculate scan_time in 100us ticks (Microsoft PTP requirement)
    if (scan_time != 0) {
        uint32_t scan = timer_elapsed32(scan_time);
        digitizer_report.scan_time = scan * DIGITIZER_SCAN_TIME_MULTIPLIER;
    }

    host_digitizer_send(&digitizer_report);
#endif

    return mouse_report;
}
#endif

void pointing_device_keycode_handler(uint16_t keycode, bool pressed) {
    if (IS_MOUSEKEY_BUTTON(keycode)) {
        uint8_t button_idx = keycode - QK_MOUSE_BUTTON_1;

        // Limit to buttons 1-3 (digitizer supports up to 3 buttons)
        if (button_idx > 2) {
            return;
        }

        uint8_t button_mask = 1 << button_idx;

        if (pressed) {
            digitizer_button_state |= button_mask;
        } else {
            digitizer_button_state &= ~button_mask;
        }

        // Send buttons to both interfaces for cross-platform compatibility:
        // - Digitizer interface: macOS expects buttons from the same interface it uses for pointing
        // - Mouse interface: Linux/other OSes use the Mouse interface for both pointing and buttons
#if defined(DIGITIZER_ENABLE)
        report_digitizer_t digitizer_report = {
            .report_id = REPORT_ID_DIGITIZER,
            .fingers = {},
            .scan_time = 0,
            .contact_count = 0,
            // .button1 = (digitizer_button_state & 0x01) ? 1 : 0,
            // .button2 = (digitizer_button_state & 0x02) ? 1 : 0,
            // .button3 = (digitizer_button_state & 0x04) ? 1 : 0,
            .reserved2 = 0
        };
        host_digitizer_send(&digitizer_report);
#endif

        report_mouse_t mouse_report = pointing_device_get_report();
        mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, pressed, button_idx);
        pointing_device_set_report(mouse_report);
        pointing_device_send();
    }
}
