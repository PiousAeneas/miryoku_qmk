// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"

// Boolean to track if Mac Mode is active
bool isMac = false; 

enum {
    // Additional Features double tap guard
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

    // Add enum members for Mac Mode tap dance
    U_TD_MAC,
    U_TD_WIN,
};

// Add custom keycodes
enum custom_keycodes {
    U_TABB = SAFE_RANGE, // Go to the previous tab
    U_TABF, // Go to the next tab
    U_BRWSR_BCK, // Shortcut for navigating back in browser
    U_BRWSR_FWD, // Shortcut for navigating forward in browser
    U_APP_BCK, // Toggle App Switcher forwards
    U_APP_FWD, // Toggle App Switcher backwards
};

void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    reset_keyboard();
  }
}

#define MIRYOKU_X(LAYER, STRING) \
void u_td_fn_U_##LAYER(tap_dance_state_t *state, void *user_data) { \
  if (state->count == 2) { \
    default_layer_set((layer_state_t)1 << U_##LAYER); \
  } \
}
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

// Mac Mode function to handle the tap dance actions for U_TD_MAC and U_TD_WIN
void u_td_mac_win_fn(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    isMac = (state->keycode == U_TD_MAC); // Toggle Mac Mode based on the keycode
    keymap_config.swap_lctl_lgui = isMac; // Swap Control and GUI on both sides based on Mac Mode state
    keymap_config.swap_rctl_rgui = isMac;
  }
}

// Add process_record_user function to handle custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        
        // Handle U_TABB and U_TABF custom keycodes
        case U_TABB:
        case U_TABF:
            if (record->event.pressed) {
                // Press and hold Left Control
                register_code(KC_LCTL);

                if (keycode == U_TABB) {
                    // If U_TABB, press and hold Left Shift
                    register_code(KC_LSFT);
                }

                // Tap Tab
                tap_code(KC_TAB);

                if (keycode == U_TABB) {
                    // If U_TABB, release Left Shift
                    unregister_code(KC_LSFT);
                }

                // Release Left Control
                unregister_code(KC_LCTL);
            }
            return false; // Skip all further processing of this key

        // Handle U_BRWSR_BCK and U_BRWSR_FWD custom keycodes
        case U_BRWSR_BCK:
        case U_BRWSR_FWD:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LGUI); // Press and hold Left GUI
                    tap_code(keycode == U_BRWSR_BCK ? KC_LBRC : KC_RBRC); // Tap [ for U_BRWSR_BCK or ] for U_BRWSR_FWD
                    unregister_code(KC_LGUI); // Release Left GUI
                } else {
                    register_code(KC_LALT); // Press and hold Left Alt
                    tap_code(keycode == U_BRWSR_BCK ? KC_LEFT : KC_RIGHT); // Tap Left Arrow for U_BRWSR_BCK or Right Arrow for U_BRWSR_FWD
                    unregister_code(KC_LALT); // Release Left Alt
                }
            }
            return false;

        case U_APP_BCK:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LGUI);
                    register_code(KC_LSFT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LALT);
                    register_code(KC_LSFT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LALT);
                }
            }
            return false;
        case U_APP_FWD:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LGUI);
                    tap_code(KC_TAB);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                }
            }
            return false;
        default:
            return true;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

    // Add Mac Mode tap dance actions
    [U_TD_MAC] = ACTION_TAP_DANCE_FN(u_td_mac_win_fn),
    [U_TD_WIN] = ACTION_TAP_DANCE_FN(u_td_mac_win_fn),

};


// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define MIRYOKU_X(LAYER, STRING) [U_##LAYER] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_##LAYER, MIRYOKU_LAYER_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};


// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t **key_overrides = (const key_override_t *[]){
    &capsword_key_override,
    NULL
};


// thumb combos

#if defined (MIRYOKU_KLUDGE_THUMBCOMBOS)
const uint16_t PROGMEM thumbcombos_base_right[] = {LT(U_SYM, KC_ENT), LT(U_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[] = {LT(U_NAV, KC_SPC), LT(U_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[] = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM thumbcombos_mouse[] = {KC_BTN2, KC_BTN1, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[] = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[] = {KC_0, KC_MINS, COMBO_END};
  #if defined (MIRYOKU_LAYERS_FLIP)
const uint16_t PROGMEM thumbcombos_sym[] = {KC_UNDS, KC_LPRN, COMBO_END};
  #else
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
  #endif
const uint16_t PROGMEM thumbcombos_fun[] = {KC_SPC, KC_TAB, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  COMBO(thumbcombos_base_right, LT(U_FUN, KC_DEL)),
  COMBO(thumbcombos_base_left, LT(U_MEDIA, KC_ESC)),
  COMBO(thumbcombos_nav, KC_DEL),
  COMBO(thumbcombos_mouse, KC_BTN3),
  COMBO(thumbcombos_media, KC_MUTE),
  COMBO(thumbcombos_num, KC_DOT),
  #if defined (MIRYOKU_LAYERS_FLIP)
  COMBO(thumbcombos_sym, KC_RPRN),
  #else
  COMBO(thumbcombos_sym, KC_LPRN),
  #endif
  COMBO(thumbcombos_fun, KC_APP)
};
#endif
