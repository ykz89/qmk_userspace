#define U_RDO C(KC_Y)
#define U_PST C(KC_V)
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)

enum charybdis_keymap_layers {
    _COLEMAK = 0,
    _NAV,
    _MOUSE,
    _MEDIA,
    _NUM,
    _FUN,
    _SYM,
};

enum layers { BASE, BUTTON, NAV, MOUSE, MEDIA, NUM, SYM, FUN };

const uint16_t PROGMEM thumbcombos_base_right[] = {LT(_SYM, KC_ENT), LT(_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[] = {LT(_NAV, KC_SPC), LT(_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[] = {KC_ENT, KC_BSPC, COMBO_END};
//const uint16_t PROGMEM thumbcombos_mouse[] = {KC_BTN1, KC_BTN3, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[] = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[] = {KC_0, KC_MINS, COMBO_END};
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
const uint16_t PROGMEM thumbcombos_fun[] = {KC_SPC, KC_TAB, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  COMBO(thumbcombos_base_right, LT(_FUN, KC_DEL)),
  COMBO(thumbcombos_base_left, LT(_MEDIA, KC_ESC)),
  COMBO(thumbcombos_nav, KC_DEL),
//  COMBO(thumbcombos_mouse, KC_BTN2),
  COMBO(thumbcombos_media, KC_MUTE),
  COMBO(thumbcombos_num, KC_DOT),
  COMBO(thumbcombos_sym, KC_LPRN),
  COMBO(thumbcombos_fun, KC_APP)
};

//#define NAV LT(_NAV, KC_SPC)
//#define MOUSE LT(_MOUSE, KC_TAB)
//#define MEDIA LT(_MEDIA, KC_ESC)
//#define NUM LT(_NUM, KC_BSPC)
//#define FUN LT(_FUN, KC_DEL)
//#define SYM LT(_SYM, KC_ENT)

//#define COLEMAK DF(_COLEMAK)
//#define GAMING DF(_GAMING)