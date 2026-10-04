// Copyright 2023 sporkus
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Thin adapter over users/ykz89/. A wrapper splices the top-row centre key
// into the shared 3x10 core, followed by the 4 thumbs.
//
// Thumb placement follows this cluster's geometry, NOT Miryoku's column order,
// and this board is the deliberate exception -- every other ykz89 board uses
// Miryoku order (see users/ykz89/ykz89.h). The 2u key ([3,4] / [3,6]) sits
// inboard and is where the thumb rests, so it takes the primary (SPC_NAV /
// BSP_NUM); the 1.25u outer key ([3,2] / [3,7]) takes the secondary (TAB_FUN /
// ENT_SYM). Read left to right that is TAB SPACE | BSPC ENT. The tertiary is a
// chord of the two on every layer (COMBO_THUMB_L/R in config.h,
// users/ykz89/ykz89_combos.c). Layer rows are the 3-thumb boards' rows minus
// the outer key, placed the same way.
//
// LAYOUT thumb order: [3,2] outer-L, [3,4] 2u-L, [3,6] 2u-R, [3,7] outer-R.
// No pointing device, so there is no pointer layer.

#include QMK_KEYBOARD_H
#include "ykz89.h"

// Centre key: tap = Slack huddle mute, hold = system mute, double-tap =
// Play/Pause. The first two are host-specific, so the key asks QMK's OS
// detection (OS_DETECTION_ENABLE in rules.mk) what it is plugged into and sends
// that host's keycode:
//
//   host        tap (Slack huddle mute)   hold (system mute)
//   macOS/iOS   Cmd+Shift+Space           KC_MUTE
//   Windows     Ctrl+Shift+Space          Win+Alt+K
//   Linux       Ctrl+Shift+Space          F20
//
// Slack's mute shortcut is an in-app one, so it only lands when Slack is the
// focused window -- which mid-huddle it usually is not. Hence the hold, which
// asks the host to mute at the source. What "at the source" means differs:
//
//   - Linux: F20 is just a free keycode, bound once on the host to
//     `wpctl set-mute @DEFAULT_AUDIO_SOURCE@ toggle` -- a real microphone mute.
//   - Windows 11: Win+Alt+K natively, though only for apps that use its
//     call-mute API.
//   - macOS: KC_MUTE, which is HID Audio Mute -- *speaker output*, not the
//     microphone. macOS has no system-wide mic mute (no hotkey, no keycode
//     QMK can send), so this is deliberately the one host where the hold mutes
//     the other direction: it is honoured out of the box with nothing to set
//     up, at the cost of silencing the huddle instead of your mic. The
//     equivalent of the other two would be F20 plus a Hammerspoon or Shortcuts
//     binding on the host.
//
// Note the macOS hold therefore duplicates the Media layer's right-thumb chord
// (COMBO_THUMB_R -> KC_MUTE, users/ykz89/ykz89_combos.c), just without the
// layer hop.
//
// Detection needs a few hundred ms after plug-in to settle, and a KVM can hide
// a switch between hosts, so both helpers treat OS_UNSURE as Linux.
#define SLACK_MUTE     LCTL(LSFT(KC_SPC))
#define SLACK_MUTE_MAC LGUI(LSFT(KC_SPC))
#define SYS_MUTE       KC_F20
#define SYS_MUTE_MAC   KC_MUTE
#define SYS_MUTE_WIN   LGUI(LALT(KC_K))

static uint16_t slack_mute_keycode(void) {
#ifdef OS_DETECTION_ENABLE
    switch (detected_host_os()) {
        case OS_MACOS:
        case OS_IOS:
            return SLACK_MUTE_MAC;
        default:
            break;
    }
#endif
    return SLACK_MUTE;
}

static uint16_t sys_mute_keycode(void) {
#ifdef OS_DETECTION_ENABLE
    switch (detected_host_os()) {
        case OS_MACOS:
        case OS_IOS:
            return SYS_MUTE_MAC;
        case OS_WINDOWS:
            return SYS_MUTE_WIN;
        default:
            break;
    }
#endif
    return SYS_MUTE;
}

enum ykz89_tap_dances {
    TD_MUTE_PLAY,
};

/** Tap -> Slack mute; held past TAPPING_TERM -> system mute; double tap -> play. */
static void mute_play_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code16((state->interrupted || !state->pressed) ? slack_mute_keycode() : sys_mute_keycode());
    } else if (state->count == 2) {
        tap_code16(KC_MPLY);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_MUTE_PLAY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mute_play_finished, NULL),
};
#define MUTE_PLAY TD(TD_MUTE_PLAY)

// clang-format off
// core (30), CENTER (1), thumbs (4) -> LAYOUT
#define LC_WRAP_IMPL(                                         \
    l00, l01, l02, l03, l04, r05, r06, r07, r08, r09,         \
    l10, l11, l12, l13, l14, r15, r16, r17, r18, r19,         \
    l20, l21, l22, l23, l24, r25, r26, r27, r28, r29,         \
    CENTER, t0, t1, t2, t3)                                   \
    l00, l01, l02, l03, l04, CENTER, r05, r06, r07, r08, r09, \
    l10, l11, l12, l13, l14,         r15, r16, r17, r18, r19, \
    l20, l21, l22, l23, l24,         r25, r26, r27, r28, r29, \
                            t0, t1,           t2, t3
#define LC_WRAP(...) LC_WRAP_IMPL(__VA_ARGS__)
#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

// Base thumb row; its innermost key per hand places QK_BOOT/EE_CLR (see ykz89.h).
#define THUMBS_BASE  TAB_FUN, SPC_NAV, BSP_NUM, ENT_SYM
#define THUMB_TUCK_L THUMB_PICK(1, THUMBS_BASE)
#define THUMB_TUCK_R THUMB_PICK(2, THUMBS_BASE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(LC_WRAP(
    HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE),
    MUTE_PLAY, THUMBS_BASE)),
  [LAYER_FUNCTION]   = LAYOUT_wrapper(LC_WRAP(LAYOUT_LAYER_FUNCTION,   _______, _______, XXXXXXX, XXXXXXX, XXXXXXX)),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LC_WRAP(LAYOUT_LAYER_NAVIGATION, _______, XXXXXXX, _______, KC_BSPC,  KC_ENT)),
  [LAYER_MEDIA]      = LAYOUT_wrapper(LC_WRAP(LAYOUT_LAYER_MEDIA,      _______, KC_MSTP, KC_MPLY, KC_MPLY, KC_MSTP)),
  [LAYER_NUMERAL]    = LAYOUT_wrapper(LC_WRAP(LAYOUT_LAYER_NUMERAL,    _______, KC_MINS,    KC_0, _______, XXXXXXX)),
  [LAYER_SYMBOLS]    = LAYOUT_wrapper(LC_WRAP(LAYOUT_LAYER_SYMBOLS,    _______, KC_UNDS, KC_RPRN, XXXXXXX, _______)),
};
// clang-format on

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [LAYER_BASE]       = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [LAYER_FUNCTION]   = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
    [LAYER_NAVIGATION] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [LAYER_MEDIA]      = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [LAYER_NUMERAL]    = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [LAYER_SYMBOLS]    = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
};
// clang-format on
#endif
