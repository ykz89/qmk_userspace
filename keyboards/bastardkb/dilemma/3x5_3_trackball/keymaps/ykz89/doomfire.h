// Copyright 2026 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "led_matrix_motion.h"

// Paints a frame of the PSX DOOM fire; a custom LED matrix animation.
bool doomfire_draw(RGB *pixels, const bklm_motion_t *m);
