// Copyright 2021 @ykz89
// SPDX-License-Identifier: GPL-2.0+

// Windows shortcuts
#define U_RDO C(KC_Y)
#define U_PST C(KC_V)
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)

// Thumb cluster mods
#define NAVI LT(NAV, KC_SPC)
#define MOUSE LT(MSE, KC_TAB)
#define MEDIA LT(MED, KC_ESC)
#define NUMB LT(NUM, KC_BSPC)
#define FUNC LT(FUN, KC_DEL)
#define SYMB LT(SYM, KC_ENT)

// Home row mods
#define HM_A	LGUI_T(KC_A)
#define HM_R	LALT_T(KC_R)
#define HM_S	LCTL_T(KC_S)
#define HM_T	LSFT_T(KC_T)
#define HM_N	LSFT_T(KC_N)
#define HM_E	LCTL_T(KC_E)
#define HM_I	LALT_T(KC_I)
#define HM_O	LGUI_T(KC_O)
#define HRMR(k1,k2,k3,k4) LSFT_T(k1),LCTL_T(k2),LALT_T(k3),LGUI_T(k4)
#define HRML(k1,k2,k3,k4) LGUI_T(k1),LALT_T(k2),LCTL_T(k3),LSFT_T(k4)

#define HRMA(k1) ALGR_T(k1)


// Default layer
#define BASE DF(0)
#define GAMING DF(1)

// Layers
#define BSE 0
#define GAM 1
#define NAV 2
#define MSE 3
#define MED 4
#define NUM 5
#define FUN 6
#define SYM 7

#define _BASE \
	KC_TAB,  KC_Q,  KC_W,  KC_F,  KC_P,  KC_B,                KC_J,   KC_L,   KC_U,   KC_Y,  KC_QUOT,KC_MINS, \
   KC_LSFT,  KC_A,  KC_R,  KC_S,  KC_T,  KC_G,                KC_M,   KC_N,   KC_E,   KC_I,  KC_O,   KC_SCLN, \
  KC_LCTRL,  KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,                KC_K,   KC_H,   KC_COMM,KC_DOT,KC_SLSH,KC_RSHIFT, \
	                      MEDIA,  NAVI, MOUSE,                SYMB,   NUMB,   FUNC
/* ,-----------------------------------------.                ,-----------------------------------------.
 * | TAB  |  Q   |  W   |  F   |  P   |  G   |                |  J   |  L   |  U   |  Y   | ' "  |   -  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |SHIFT |  A   |  R   |  S   |  T   |  D   |                |  H   |  N   |  E   |  I   |  O   |   ;  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * | CTRL |  Z   |  X   |  C   |  V   |  B   |                |  K   |  M   | , <  | . >  | / ?  |SHIFT |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |MEDIA |  NAV |MOUSE |  | SYM  | NUM  | FUN  |
 *                             `--------------------'  `--------------------'
 */

#define _GAME \
    KC_TAB,  KC_Q,  KC_W,  KC_F,  KC_P,  KC_G,                KC_J,    KC_L,  KC_U,   KC_Y,  KC_QUOT,KC_MINS, \
   KC_LSFT,  KC_A,  KC_R,  KC_S,  KC_T,  KC_D,                KC_H,    KC_N,  KC_E,   KC_I,  KC_O,   KC_SCLN, \
  KC_LCTRL,  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,                KC_K,    KC_M,  KC_COMM,KC_DOT,KC_SLSH,KC_RSHIFT, \
	                      MEDIA,  NAVI, MOUSE,                SYMB,   NUMB,   FUNC
/* ,-----------------------------------------.                ,-----------------------------------------.
 * | TAB  |  Q   |  W   |  F   |  P   |  G   |                |  J   |  L   |  U   |  Y   | ' "  |   -  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |SHIFT |  A   |  R   |  S   |  T   |  D   |                |  H   |  N   |  E   |  I   |  O   |   ;  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * | CTRL |  Z   |  X   |  C   |  V   |  B   |                |  K   |  M   | , <  | . >  | / ?  |SHIFT |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |MEDIA |  NAV |MOUSE |  | SYM  | NUM  | FUN  |
 *                             `--------------------'  `--------------------'
 */

#define _NAVI \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND, XXXXXXX, \
    XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,     KC_CAPS, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, XXXXXXX, \
	XXXXXXX, XXXXXXX, KC_ALGR, XXXXXXX, XXXXXXX, XXXXXXX,      KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, XXXXXXX, \
	                           XXXXXXX, XXXXXXX, XXXXXXX,      KC_ENT, KC_BSPC, KC_DEL
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |      |      |      |      |      |                | REDO |PASTE | COPY | CUT  | UNDO |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  GUI |  ALT | CTRL | SHFT |      |                | CAPS | LEFT | DOWN |  UP  | LEFT |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |      | ALGR |      |      |      |                | INS  | HOME | PGDN | PGUP | END  |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |      |      |      |  |ENTER |BCKSP | DEL  |
 *                             `--------------------'  `--------------------'
 */

#define _MOUS \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND, XXXXXXX, \
    XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX, \
 	XXXXXXX, XXXXXXX, KC_ALGR, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, XXXXXXX, \
 	                           XXXXXXX, XXXXXXX, XXXXXXX,       KC_ENT, KC_BSPC, KC_DEL
 /* ,-----------------------------------------.                ,-----------------------------------------.
  * |      |      |      |      |      |      |                | REDO |PASTE | COPY | CUT  | UNDO |      |
  * |------+------+------+------+------+------|                |------+------+------+------+------+------|
  * |      |  GUI |  ALT | CTRL | SHFT |      |                | CAPS | LEFT | DOWN |  UP  | LEFT |      |
  * |------+------+------+------+------+------|                |------+------+------+------+------+------|
  * |      |      | ALGR |      |      |      |                | INS  | HOME | PGDN | PGUP | END  |      |
  * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
  *                             |      |      |      |  |ENTER |BCKSP | DEL  |
  *                             `--------------------'  `--------------------'
  */

#define _NUMB \
	XXXXXXX,KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	XXXXXXX,KC_SCLN, KC_4, KC_5, KC_6,  KC_EQL,                 XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, \
	XXXXXXX, KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,                 XXXXXXX, XXXXXXX, XXXXXXX, KC_ALGR, XXXXXXX, XXXXXXX, \
	                     KC_DOT, KC_0, KC_MINS,                 XXXXXXX, XXXXXXX, XXXXXXX
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  [   |  7   |  8   |  9   |  ]   |                |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  ;   |  4   |  5   |  6   |  =   |                |      | SHFT | CTRL | ALT  |  GUI |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  `   |  1   |  2   |  3   |  \   |                |      |      |      | ALGR |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |  .   |  0   |  -   |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */


#define _FUNC \
	XXXXXXX, KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR,            QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	XXXXXXX, KC_F11, KC_F4, KC_F5, KC_F6, KC_SLCK,            XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, \
	XXXXXXX, KC_F10, KC_F1, KC_F2, KC_F3, KC_PAUS,            XXXXXXX, XXXXXXX, XXXXXXX, KC_ALGR, XXXXXXX, XXXXXXX, \
	                       KC_APP,KC_SPC,  KC_TAB,            XXXXXXX, XXXXXXX, XXXXXXX
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  F12 |  F7  |  F8  |  F9  | prtsc|                |RESET |      |      |      |      |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  F11 |  F4  |  F5  |  F6  | scrlk|                |      | SHFT | CTRL | ALT  |  GUI |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  F10 |  F1  |  F2  |  F3  | pause|                |      |      |      | ALGR |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |contxt| space| tab  |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */

#define _SYMB \
	XXXXXXX, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	XXXXXXX, KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, \
	XXXXXXX, KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE,     XXXXXXX, XXXXXXX, XXXXXXX, KC_ALGR, XXXXXXX, XXXXXXX, \
	                           KC_LPRN, KC_RPRN, KC_UNDS,     XXXXXXX, XXXXXXX, XXXXXXX
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  [   |  &   |  *   |  (   |  ]   |                |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  :   |  $   |  %   |  ^   |  +   |                |      | SHFT | CTRL | ALT  |  GUI |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  ~   |  !   |  @   |  #   |  |   |                |      |      |      | ALGR |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |  (   |   )  |  _   |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */

// 4x6_3 home row mod and tap hold wrapper
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
 	      k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
 	      k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
 	      k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
 	                     k37, k38, k39,    k40, k41, k42 \
) \
 	k01,      k02, k03, k04, k05,  k06,    k07,      k08, k09, k10, k11,  k12, \
 	k13, HRML(k14, k15, k16, k17), k18,    k19, HRMR(k20, k21, k22, k23), k24, \
 	k25, k26, HRMA(k27), k28, k29,  k30,   k31,  k32, k33, HRMA(k34), k35, k36, \
 	                    k37, k38,  k39,    k40, k41, k42

// 36-key to 4x6_6 conversion
#define C_60(k) SPLIT_36_TO_60(k)
#define SPLIT_36_TO_60( \
	k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
	               k37, k38, k39,    k40, k41, k42 \
) \
    KC_ESC, KC_1,KC_2,KC_3,KC_4,KC_5,                    KC_6,KC_7,KC_8,KC_9,KC_0,KC_GRV, \
	k01,    k02, k03, k04, k05, k06,                      k07, k08, k09, k10, k11, k12, \
    k13,    k14, k15, k16, k17, k18,                      k19, k20, k21, k22, k23, k24, \
    k25,    k26, k27, k28, k29, k30, KC_LBRC,   KC_RBRC,  k31, k32, k33, k34, k35, k36, \
         	  XXXXXXX, XXXXXXX, k37, k38, k39, k40, k41, k42, XXXXXXX, XXXXXXX \

// Layout aliases
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_60key_w(...) LAYOUT(__VA_ARGS__)