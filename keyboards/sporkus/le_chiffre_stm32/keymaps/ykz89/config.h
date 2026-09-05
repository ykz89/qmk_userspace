// Copyright 2023 sporkus
// Copyright 2024 @ykz89
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Both hands lack the tertiary thumb: chord it on every layer (ykz89_combos.c).
#define COMBO_THUMB_L
#define COMBO_THUMB_R
// The 50 ms default is tight for a two-thumb roll.
#define COMBO_TERM 60

// Chordal Hold handedness comes from the "hand" annotations in keyboard.json.
#define CHORDAL_HOLD

#define TAPPING_TERM 200

// RGB animations; the board enables only CYCLE_ALL. Its config.h is included
// first, hence the #undef.
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN

#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_WAVE
#define ENABLE_RGB_MATRIX_PIXEL_FLOW
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_STARLIGHT
#define ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
#define ENABLE_RGB_MATRIX_RIVERFLOW

#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH

#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_HUE_WAVE
#undef RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_VAL 100
