// Stand-in for BastardKB's bk_pointing_modes.h. Values must match the real enum
// (modules/bastardkb/bk_pointing_device/bk_pointing_modes.h); run.py checks.
#pragma once
#include <stdint.h>
enum {
    MODE_NORMAL = 0,
    MODE_SNIPING = 1,
    MODE_DRAGSCROLL = 2,
    MODE_CURSOR = 3,
    MODE_BRIGHTNESS = 4,
    MODE_ZOOM = 5,
    MODE_VOLUME = 6,
    MODE_TAB_SWITCH = 7,
    MODE_HISTORY = 8,
    MODE_CUSTOM1 = 9,
    MODE_CUSTOM2 = 10,
    MODE_CUSTOM3 = 11,
    MODE_CUSTOM4 = 12,
    MODE_CUSTOM5 = 13,
    MODE_LAST = 14
};
uint8_t bkpd_mode_get_active_id(void);
