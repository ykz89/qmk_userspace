// Copyright 2026 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later
//
// LED matrix simulator: runs the real drawing code of modules/bastardkb/bk_led_matrix
// on the host, with QMK stubbed and the bit-banged display (led_matrix_display.c)
// replaced by a frame recorder. Timing matches the keyboard: housekeeping every
// 1 ms, a pointer report every 10 ms (POINTING_DEVICE_TASK_THROTTLE_MS).
// The simulated half has both the panel and USB; split sync is not simulated.
//
//   sim <scenario> <out.bin> [style]   one scenario per process, so module state starts clean;
//                                      style is a trackball animation name, or userN for the
//                                      keymap's bklm_motion_user() (default: the module's)
//   sim --list                         scenario names, one per line
//   sim --styles                       trackball animation names, one per line
//
// out.bin: "BKLM", u16 cols, u16 rows, then per frame u32 time_ms + cols*rows*3 bytes RGB,
// row-major from the top row, at full scale (before the firmware's brightness divisor).

#include <stdio.h>

#include "quantum.h"
#include "transactions.h"
#include "argos_rgb.h"
#include "bk_pointing_modes.h"

#include "led_matrix.h"
#include "led_matrix_display.h"
#include "led_matrix_motion.h"
#include "introspection.h"

/* --- QMK stand-ins ------------------------------------------------------- */

static uint32_t now_ms;
static uint8_t  sim_mods;
static uint8_t  sim_pointer_mode = MODE_NORMAL;
layer_state_t   layer_state;

uint32_t timer_read32(void) { return now_ms; }
uint32_t timer_elapsed32(uint32_t last) { return now_ms - last; }
void     wait_us(uint32_t us) { (void)us; }
bool     is_keyboard_master(void) { return true; }
bool     is_keyboard_left(void) { return true; }
uint8_t  get_mods(void) { return sim_mods; }
uint8_t  rgb_matrix_get_val(void) { return RGB_MATRIX_MAXIMUM_BRIGHTNESS; }
uint8_t  bkpd_mode_get_active_id(void) { return sim_pointer_mode; }
static uint8_t sim_argos[8];
void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size) { memcpy(buf, &sim_argos[offset], size); }
void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size) { memcpy(&sim_argos[offset], buf, size); }
void     transaction_register_rpc(int8_t id, slave_callback_t cb) { (void)id, (void)cb; }
bool     transaction_rpc_send(int8_t id, uint8_t len, const void *data) { return (void)id, (void)len, (void)data, true; }

uint8_t get_highest_layer(layer_state_t state) {
    for (int8_t i = 31; i > 0; i--) {
        if (state & (1UL << i)) return (uint8_t)i;
    }
    return 0;
}

/* Layer colours live in the keyboard's Argos settings, which the simulator
 * cannot read: use evenly spread hues instead. */
void argos_rgb_get_layer_color(uint8_t layer, RGB *rgb) {
    *rgb = hsv_to_rgb((HSV){.h = (uint8_t)(layer * 36), .s = 255, .v = 255});
}

/* --- Display stand-in: record frames instead of bit-banging -------------- */

static FILE   *out;
static uint8_t idle_divisor = 1;
static int     frames;

void bklm_set_idle_brightness_divisor(uint8_t divisor) { idle_divisor = divisor ? divisor : 1; }
void bklm_init(void) {}

void bklm_show(const RGB *pixels) {
    if (pixels == NULL) return;
    fwrite(&now_ms, sizeof(now_ms), 1, out);
    for (int y = BKLM_ROWS - 1; y >= 0; y--) { /* framebuffer y = 0 is the bottom row */
        for (int x = 0; x < BKLM_COLS; x++) {
            const RGB *p      = &pixels[y * BKLM_COLS + x];
            uint8_t    rgb[3] = {p->r / idle_divisor, p->g / idle_divisor, p->b / idle_divisor};
            fwrite(rgb, 1, 3, out);
        }
    }
    frames++;
}

/* --- Module entry points (normally called by QMK's community module glue) - */

void           keyboard_post_init_bk_led_matrix(void);
void           housekeeping_task_bk_led_matrix(void);
bool           process_record_bk_led_matrix(uint16_t keycode, keyrecord_t *record);
report_mouse_t pointing_device_task_bk_led_matrix(report_mouse_t mouse_report);

/* --- Scenarios ----------------------------------------------------------- */

enum { L_BASE = 0, L_FUN, L_NAV, L_MEDIA, L_PTR, L_NUM, L_SYM }; /* users/ykz89/ykz89.h order */
#define SHIFT 0x02
#define CTRL 0x01

typedef struct {
    uint32_t ms;      /* duration */
    int32_t  vx, vy;  /* trackball velocity, counts per second (x right, y down) */
    uint8_t  layer;   /* highest active layer */
    uint8_t  mods;
    uint8_t  pointer; /* bk_pointing_device mode */
    uint16_t press;   /* keycode tapped at the start of the segment, 0 for none */
} segment_t;

typedef struct {
    const char      *name;
    const segment_t *seg;
    int              n;
} scenario_t;

#define SCENARIO(name, ...) {name, (const segment_t[]){__VA_ARGS__}, sizeof((const segment_t[]){__VA_ARGS__}) / sizeof(segment_t)}

/* Polar helper for the circle: 16 headings, 250 ms each. */
#define HEADING(c, s) {250, (c) * 25, (s) * 25, 0, 0, 0}

static const scenario_t scenarios[] = {
    SCENARIO("idle_duck", {3000, 0, 0, 0, 0, 0}),
    /* Ball still for 6 s: the idle animation (or the duck with MOTION_IDLE 0). */
    SCENARIO("idle_long", {6000, 0, 0, 0, 0, 0}),
    /* Half a minute still, for slow-burn things (e.g. how often the Asteroids ship dies). */
    SCENARIO("idle_30s", {30000, 0, 0, 0, 0, 0}),
    SCENARIO("roll_right", {400, 0, 0, 0, 0, 0}, {1500, 2000, 0, 0, 0, 0}, {900, 0, 0, 0, 0, 0}),
    SCENARIO("roll_up", {400, 0, 0, 0, 0, 0}, {1500, 0, -2000, 0, 0, 0}, {900, 0, 0, 0, 0, 0}),
    SCENARIO("roll_diagonal", {400, 0, 0, 0, 0, 0}, {1500, -1500, 1500, 0, 0, 0}, {900, 0, 0, 0, 0, 0}),
    SCENARIO("slow_then_fast", {300, 0, 0, 0, 0, 0}, {1200, 300, 0, 0, 0, 0}, {1200, 6000, 0, 0, 0, 0}, {900, 0, 0, 0, 0, 0}),
    SCENARIO("circle", {300, 0, 0, 0, 0, 0},
             HEADING(80, 0), HEADING(74, 31), HEADING(57, 57), HEADING(31, 74), HEADING(0, 80), HEADING(-31, 74), HEADING(-57, 57), HEADING(-74, 31),
             HEADING(-80, 0), HEADING(-74, -31), HEADING(-57, -57), HEADING(-31, -74), HEADING(0, -80), HEADING(31, -74), HEADING(57, -57), HEADING(74, -31),
             {900, 0, 0, 0, 0, 0}),
    SCENARIO("layers", {600, 0, 0, L_BASE, 0, 0}, {900, 0, 0, L_NAV, 0, 0}, {300, 0, 0, L_BASE, 0, 0}, {900, 0, 0, L_NUM, 0, 0}, {300, 0, 0, L_BASE, 0, 0}, {900, 0, 0, L_SYM, 0, 0}, {600, 0, 0, L_BASE, 0, 0}),
    /* Each layer held for 2.5 s: the layer animations. */
    SCENARIO("layer_fun", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_FUN, 0, 0}),
    SCENARIO("layer_nav", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_NAV, 0, 0}),
    SCENARIO("layer_media", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_MEDIA, 0, 0}),
    SCENARIO("layer_ptr", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_PTR, 0, 0}),
    SCENARIO("layer_num", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_NUM, 0, 0}),
    SCENARIO("layer_sym", {300, 0, 0, L_BASE, 0, 0}, {2500, 0, 0, L_SYM, 0, 0}),
    SCENARIO("modifiers", {500, 0, 0, 0, 0, 0}, {800, 0, 0, 0, SHIFT, 0}, {800, 0, 0, 0, SHIFT | CTRL, 0}, {500, 0, 0, 0, 0, 0}),
    SCENARIO("pointer_modes", {500, 0, 0, 0, 0, 0}, {1000, 0, 0, L_PTR, 0, MODE_SNIPING}, {1000, 0, 0, L_PTR, 0, MODE_DRAGSCROLL}, {500, 0, 0, 0, 0, 0}),
    /* Tap LM_ANIM every 1.2 s with the ball still: each style previews itself. */
    SCENARIO("cycle_animations", {300, 0, 0, 0, 0, 0, 0},
             {1200, 0, 0, 0, 0, 0, LM_ANIM}, {1200, 0, 0, 0, 0, 0, LM_ANIM}, {1200, 0, 0, 0, 0, 0, LM_ANIM},
             {1200, 0, 0, 0, 0, 0, LM_ANIM}, {1200, 0, 0, 0, 0, 0, LM_ANIM}, {1200, 0, 0, 0, 0, 0, LM_ANIM}),
    /* A second of rolling, then three still: shows what keeps going after you stop. */
    SCENARIO("roll_then_rest", {300, 0, 0, 0, 0, 0, 0}, {1000, 1500, -1500, 0, 0, 0, 0}, {3000, 0, 0, 0, 0, 0, 0}),
    /* Left for a second, then right, then still: what a slosh looks like. */
    SCENARIO("left_then_right", {300, 0, 0, 0, 0, 0, 0}, {800, -2500, 0, 0, 0, 0, 0}, {800, 2500, 0, 0, 0, 0, 0}, {3000, 0, 0, 0, 0, 0, 0}),
    SCENARIO("roll_on_nav_layer", {400, 0, 0, L_NAV, 0, 0}, {1500, 2000, -1000, L_NAV, 0, 0}, {900, 0, 0, L_NAV, 0, 0}),
};
#define N_SCENARIOS (int)(sizeof(scenarios) / sizeof(scenarios[0]))

static void key_event(uint16_t keycode) {
    keyrecord_t rec = {.event = {.pressed = true}};
    process_record_bk_led_matrix(keycode, &rec);
    rec.event.pressed = false;
    process_record_bk_led_matrix(keycode, &rec);
}

static void run(const scenario_t *sc) {
    int32_t  rem_x = 0, rem_y = 0; /* sub-count remainders, so slow moves still add up */
    uint8_t  layer = 0, mods = 0;
    now_ms         = 1000;
    keyboard_post_init_bk_led_matrix();

    for (int s = 0; s < sc->n; s++) {
        const segment_t *g = &sc->seg[s];
        if (g->layer != layer || g->mods != mods) key_event(0);
        if (g->press) key_event(g->press);
        layer            = g->layer;
        mods             = g->mods;
        layer_state      = layer ? (1UL << layer) : 0;
        sim_mods         = mods;
        sim_pointer_mode = g->pointer;

        for (uint32_t t = 0; t < g->ms; t++, now_ms++) {
            if (now_ms % 10 == 0 && (g->vx || g->vy)) {
                rem_x += g->vx * 10;
                rem_y += g->vy * 10;
                report_mouse_t r = {.x = (int16_t)(rem_x / 1000), .y = (int16_t)(rem_y / 1000)};
                rem_x %= 1000;
                rem_y %= 1000;
                pointing_device_task_bk_led_matrix(r);
            }
            housekeeping_task_bk_led_matrix();
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--list") == 0) {
        for (int i = 0; i < N_SCENARIOS; i++) puts(scenarios[i].name);
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--styles") == 0) {
        for (uint8_t i = 0; i < BKLM_MOTION_STYLE_COUNT; i++) puts(bklm_motion_style_name(i));
        for (uint8_t i = 0; i < LED_MATRIX_MODULE_MOTION_USER_COUNT; i++) printf("user%u\n", i);
        return 0;
    }
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "usage: %s <scenario> <out.bin> [style] | --list | --styles\n", argv[0]);
        return 2;
    }
    if (argc == 4) {
        uint8_t i = 0;
        unsigned user;
        while (i < BKLM_MOTION_STYLE_COUNT && strcmp(argv[3], bklm_motion_style_name(i)) != 0) i++;
        if (i < BKLM_MOTION_STYLE_COUNT) {
            bklm_motion_style = i;
        } else if (sscanf(argv[3], "user%u", &user) == 1 && user < LED_MATRIX_MODULE_MOTION_USER_COUNT) {
            bklm_motion_style = BKLM_MOTION_USER + user;
        } else {
            fprintf(stderr, "unknown style %s (try --styles)\n", argv[3]);
            return 2;
        }
    }
    for (int i = 0; i < N_SCENARIOS; i++) {
        if (strcmp(argv[1], scenarios[i].name) != 0) continue;
        out = fopen(argv[2], "wb");
        if (!out) return perror(argv[2]), 1;
        const uint16_t dims[2] = {BKLM_COLS, BKLM_ROWS};
        fwrite("BKLM", 1, 4, out);
        fwrite(dims, sizeof(dims), 1, out);
        run(&scenarios[i]);
        fclose(out);
        fprintf(stderr, "%s: %d frames\n", argv[1], frames);
        return 0;
    }
    fprintf(stderr, "unknown scenario %s (try --list)\n", argv[1]);
    return 2;
}
