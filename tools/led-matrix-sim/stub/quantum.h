// Stand-in for QMK's quantum.h: just what modules/bastardkb/bk_led_matrix uses.
// The simulator (../sim.c) implements every function declared here.
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "color.h" // QMK's real one (RGB, HSV, hsv_to_rgb), from the QMK tree

#define ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(major, minor, patch) _Static_assert(1, "")
#ifndef ARRAY_SIZE // QMK's util.h (via color.h) usually has these already
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif
#ifndef MIN
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#    define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef CONSTRAIN
#    define CONSTRAIN(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#endif

/* Timer: a virtual millisecond clock the simulator advances. */
uint32_t timer_read32(void);
uint32_t timer_elapsed32(uint32_t last);
void     wait_us(uint32_t us);

/* Split: the simulated panel half is also the USB half (cable on the left). */
bool is_keyboard_master(void);
bool is_keyboard_left(void);

#define MOD_BIT(code) (1 << ((code) & 0x07))
#define MOD_MASK_CTRL 0x11
#define MOD_MASK_SHIFT 0x22
#define MOD_MASK_ALT 0x44
#define MOD_MASK_GUI 0x88
uint8_t get_mods(void);
typedef uint32_t layer_state_t;
extern layer_state_t layer_state;
uint8_t get_highest_layer(layer_state_t state);

uint8_t rgb_matrix_get_val(void);

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 176

/* GPIO: unused by the simulator's display stand-in. */
#define gpio_set_pin_output(pin) ((void)(pin))
#define gpio_write_pin_low(pin) ((void)(pin))

/* Pointer reports and key records: only the fields the module reads. */
typedef struct {
    uint8_t buttons;
    int16_t x, y, h, v;
} report_mouse_t;
typedef struct {
    struct {
        bool pressed;
    } event;
} keyrecord_t;
