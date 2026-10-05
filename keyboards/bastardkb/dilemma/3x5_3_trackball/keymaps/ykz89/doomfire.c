// Copyright 2026 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "led_matrix_motion.h"
#include "doomfire.h"

/*
 * The PSX DOOM fire (https://fabiensanglard.net/doom_fire_psx/): a hidden row
 * of full heat under the panel; each step every cell takes the heat of the one
 * below it, a little cooler and nudged sideways at random. Rolling the ball
 * sideways blows the flames that way, rolling it up stokes them; when the
 * animation fades out, the fire is starved and dies down.
 */

#define DOOMFIRE_HEAT    36
#define DOOMFIRE_STEP_MS 35

static const uint8_t doomfire_palette[DOOMFIRE_HEAT + 1][3] = {
    {0x07, 0x07, 0x07}, {0x1F, 0x07, 0x07}, {0x2F, 0x0F, 0x07}, {0x47, 0x0F, 0x07}, {0x57, 0x17, 0x07}, {0x67, 0x1F, 0x07},
    {0x77, 0x1F, 0x07}, {0x8F, 0x27, 0x07}, {0x9F, 0x2F, 0x07}, {0xAF, 0x3F, 0x07}, {0xBF, 0x47, 0x07}, {0xC7, 0x47, 0x07},
    {0xDF, 0x4F, 0x07}, {0xDF, 0x57, 0x07}, {0xDF, 0x57, 0x07}, {0xD7, 0x5F, 0x07}, {0xD7, 0x5F, 0x07}, {0xD7, 0x67, 0x0F},
    {0xCF, 0x6F, 0x0F}, {0xCF, 0x77, 0x0F}, {0xCF, 0x7F, 0x0F}, {0xCF, 0x87, 0x17}, {0xC7, 0x87, 0x17}, {0xC7, 0x8F, 0x17},
    {0xC7, 0x97, 0x1F}, {0xBF, 0x9F, 0x1F}, {0xBF, 0x9F, 0x1F}, {0xBF, 0xA7, 0x27}, {0xBF, 0xA7, 0x27}, {0xBF, 0xAF, 0x2F},
    {0xB7, 0xAF, 0x2F}, {0xB7, 0xB7, 0x2F}, {0xB7, 0xB7, 0x37}, {0xCF, 0xCF, 0x6F}, {0xDF, 0xDF, 0x9F}, {0xEF, 0xEF, 0xC7},
    {0xFF, 0xFF, 0xFF},
};

bool doomfire_draw(RGB *pixels, const bklm_motion_t *m) {
    static uint8_t  heat[BKLM_ROWS + 1][BKLM_COLS]; /* row 0 at the top; the last row is the hidden source */
    static uint32_t step_acc;

    if (m->fresh) {
        memset(heat, 0, sizeof(heat));
        step_acc = DOOMFIRE_STEP_MS;
    }
    for (uint8_t x = 0; x < BKLM_COLS; x++) heat[BKLM_ROWS][x] = (uint8_t)(DOOMFIRE_HEAT * m->fade / 255);

    /* Wind from rolling sideways, -2..2 cells a step; rolling up cools the flames less. */
    const int8_t  wind  = (int8_t)CONSTRAIN(m->vx / 1500, -2, 2);
    const uint8_t stoke = m->vy > 0 ? (uint8_t)MIN(m->vy / 1000, 2) : 0;

    for (step_acc += m->dt; step_acc >= DOOMFIRE_STEP_MS; step_acc -= DOOMFIRE_STEP_MS) {
        for (uint8_t row = 1; row <= BKLM_ROWS; row++) {
            for (uint8_t x = 0; x < BKLM_COLS; x++) {
                const uint8_t h = heat[row][x];
                const uint8_t r = (uint8_t)(bklm_rand() & 3);
                /* The original drifts -2..1; the wind shifts that. */
                const int8_t  dst  = (int8_t)((x - r + 1 + wind + 2 * BKLM_COLS) % BKLM_COLS);
                const uint8_t cool = (uint8_t)((r & 1) + (bklm_rand() % (6 - stoke))); /* ~3 a row calm, ~2 stoked: 16 rows, not 168 */
                heat[row - 1][dst] = h > cool ? h - cool : 0;
            }
        }
    }

    for (uint8_t row = 0; row < BKLM_ROWS; row++) {
        for (uint8_t x = 0; x < BKLM_COLS; x++) {
            const uint8_t h = heat[row][x];
            if (h == 0) continue; /* the palette's near-black stays off */
            *bklm_px(pixels, x, (uint8_t)(BKLM_ROWS - 1 - row)) = (RGB){.r = doomfire_palette[h][0], .g = doomfire_palette[h][1], .b = doomfire_palette[h][2]};
        }
    }
    return true;
}
