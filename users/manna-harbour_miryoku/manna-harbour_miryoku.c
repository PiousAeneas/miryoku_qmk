// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"

// Boolean to track if Mac Mode is active
bool isMac = false; 

// Tap dance actions enum
enum {
    // Additional Features double tap guard
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

    // Mac Mode
    U_TD_MAC,
    U_TD_WIN,

    // App Switcher
    U_TD_APP_BCK,
    U_TD_APP_FWD,
};

// Add custom keycodes
enum custom_keycodes {
    // Tab navigation
    U_TABB = SAFE_RANGE,
    U_TABF,

    // Browser navigation
    U_BRWSR_BCK,
    U_BRWSR_FWD,

    // App Switcher
    U_APP_BCK,
    U_APP_FWD,
};

// Custom Functions
// Additional Features double tap guard
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

// Mac Mode
// Turn on Mac Mode
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    isMac = true;
    keymap_config.swap_lctl_lgui = true; // Swap Left Control and GUI
    keymap_config.swap_rctl_rgui = true; // Swap Right Control and GUI
  }
}
// Turn off Mac Mode
void u_td_win_fn(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    isMac = false;
    keymap_config.swap_lctl_lgui = false;
    keymap_config.swap_rctl_rgui = false;
  }
}

// App Switcher
// U_APP_FWD
void u_td_app_fwd_fn(tap_dance_state_t *state, void *user_data) {
    uint16_t modifier = isMac ? KC_LGUI : KC_LALT; // Determine modifier based on OS
    if (state->count == 1) {
        register_code(modifier); // Single tap: Register modifier
        tap_code(KC_TAB); // Tap Tab to switch to the next application
    } else {
        unregister_code(modifier); // Double tap: Unregister the modifier key
    }
}
// U_APP_BCK
void u_td_app_bck_fn(tap_dance_state_t *state, void *user_data) {
    uint16_t modifier = isMac ? KC_LGUI : KC_LALT;
    if (state->count == 1) {
        register_code(modifier);
        register_code(KC_LSFT);
        tap_code(KC_TAB);
    } else {
        unregister_code(KC_LSFT);
        unregister_code(modifier);
    }
}


// Tap dance actions array
tap_dance_action_t tap_dance_actions[] = {

    // Additional Features double tap guard
    [U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

    // Mac Mode tap dance actions
    [U_TD_MAC] = ACTION_TAP_DANCE_FN(u_td_mac_fn),
    [U_TD_WIN] = ACTION_TAP_DANCE_FN(u_td_win_fn),

    // App Switcher tap dance actions
    [U_TD_APP_BCK] = ACTION_TAP_DANCE_FN(u_td_app_bck_fn), // Tap dance action for U_APP_BCK
    [U_TD_APP_FWD] = ACTION_TAP_DANCE_FN(u_td_app_fwd_fn), // Tap dance action for U_APP_FWD
};

// Add process_record_user function to handle custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        
        // Tab navigation with U_TABB and U_TABF
        case U_TABB:
        case U_TABF:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                if (keycode == U_TABB) {
                    register_code(KC_LSFT); // If U_TABB, press and hold Left Shift
                }
                tap_code(KC_TAB); // Tap Tab
                if (keycode == U_TABB) {
                    unregister_code(KC_LSFT);
                }
                unregister_code(KC_LCTL);
            }
            return false;

        // Browser navigation with U_BRWSR_BCK and U_BRWSR_FWD
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

        // Handle U_APP_BCK and U_APP_FWD custom keycodes
        case U_APP_BCK:
        case U_APP_FWD: {
            tap_dance_action_t *action = &tap_dance_actions[keycode == U_APP_BCK ? U_TD_APP_BCK : U_TD_APP_FWD]; // Initialize tap dance action based on keycode
            if (record->event.pressed) {
                action->state.count = 0; // Reset tap count
                action->state.pressed = true; // Set the action as pressed
                tap_dance_pair_on_each_tap(&action->state, action->user_data); // Process the tap dance action on each tap
            } else {
                action->state.pressed = false; // Set the action as not pressed
                reset_tap_dance(&action->state); // Reset the tap dance state for next usage
            }
            return false; // Indicate that the key event has been fully processed
        }
        
        default:
            return true;
    }
}

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
