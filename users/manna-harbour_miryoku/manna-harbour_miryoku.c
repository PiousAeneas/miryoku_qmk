// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"

/*
    Edited manna-harbour_miryoku.c to add the following custom actions:
    1. Mac Mode via U_TD_MAC and U_TD_WIN.
    2. OS-Specific Tab Navigation via U_TABB and U_TABF.
    3. OS-Specific Browser Navigation via U_BRWSR_BCK and U_BRWSR_FWD.
    4. OS-Specific Spotlight Search via U_SEARCH.
    5. Set Windows as default Clipboard by overriding definitions from manna-harbour_miryoku.h
        since Mac Mode swaps Control and GUI for Paste, Copy, Cut, and Undo.
    6. OS-Specific Redo via process_record_user intercept for U_RDO
    7. OS-Specific Paste and Paste Special tap dance to replace U_PST
    8. OS-Specific em dash symbol on hyphen double tap.
    9. Excel Shortcuts: New Window, Freeze, Indent, Outdent, Add Decimal, Remove Decimal
    10. OS-Specific Screenshot tap dance using U_TD_PSCR.
*/

// Set Windows as default Clipboard by overriding manna-harbour_miryoku.h
#undef  U_RDO
#undef  U_PST // U_PST replaced by Paste Special tap dance.
#undef  U_CPY
#undef  U_CUT
#undef  U_UND
#define U_RDO C(KC_Y)
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)

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

    // Paste Special
    U_TD_PST,

    // Screenshot
    U_TD_PSCR,

    // Em Dash
    U_TD_EMDASH,
};

// Add custom keycodes
enum custom_keycodes {
    // Tab navigation
    U_TABB = SAFE_RANGE,
    U_TABF,

    // Browser navigation
    U_BRWSR_BCK,
    U_BRWSR_FWD,

    U_SEARCH, // "Spotlight" search

    // Excel Shortcuts
    U_XWIN,
    U_XFRZ,
    U_XIND,
    U_XOUT,
    U_XDECINC,
    U_XDECDEC,
};

// CUSTOM FUNCTIONS
// Additional Features double tap guard
void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    tap_code(QK_BOOT); # Replaced deprecated reset_keyboard()
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

// Mac Mode tap dance action
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

// Paste Special tap dance action
void u_pst_fn(void) { // Paste helper function
    if (isMac) { // Send Cmd+V for Mac
        register_code(KC_LCMD);
        tap_code(KC_V);
        unregister_code(KC_LCMD);
    } else { // Send Ctrl+V for Windows
        register_code(KC_LCTL);
        tap_code(KC_V);
        unregister_code(KC_LCTL);
    }
}
void u_pst_sp_fn(void) { // Paste Special helper function
    if (isMac) { // Send Shift+Opt+Cmd+V for Mac
        register_code(KC_LSFT);
        register_code(KC_LALT);
        register_code(KC_LCMD);
        tap_code(KC_V);
        unregister_code(KC_LCMD);
        unregister_code(KC_LALT);
        unregister_code(KC_LSFT);
    } else { // Send Ctrl+Shift+V for Windows
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        tap_code(KC_V);
        unregister_code(KC_LSFT);
        unregister_code(KC_LCTL);
    }
}
void u_td_pst_sp_fn(tap_dance_state_t *state, void *user_data) {
    switch (state->count) { // Paste Special tap dance action
        case 1:
            u_pst_fn();
            break;
        case 2:
            u_pst_sp_fn();
            break;
    }
}

// Screenshot tap dance action
void u_td_pscr_fn(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (isMac) {
                register_code(KC_LSFT);
                register_code(KC_LCMD);
                tap_code(KC_4);
                unregister_code(KC_LCMD);
                unregister_code(KC_LSFT);
            } else {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                tap_code(KC_S);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
            }
            break;
        case 2:
            if (isMac) {
                register_code(KC_LSFT);
                register_code(KC_LCMD);
                tap_code(KC_3);
                unregister_code(KC_LCMD);
                unregister_code(KC_LSFT);
            } else {
                tap_code(KC_PSCR);
            }
            break;
    }
}

// Em Dash tap dance action
void u_td_emdash_fn(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap_code(KC_MINS);
            break;
        case 2:
            if (isMac) {
                register_code(KC_LALT);
                register_code(KC_LSFT);
                tap_code(KC_MINS);
                unregister_code(KC_LSFT);
                unregister_code(KC_LALT);
            } else {
                register_code(KC_LALT);
                tap_code(KC_KP_0);
                tap_code(KC_KP_1);
                tap_code(KC_KP_5);
                tap_code(KC_KP_1);
                unregister_code(KC_LALT);
            }
            break;
    }
}

// TAP DANCE ACTIONS ARRAY
tap_dance_action_t tap_dance_actions[] = {

    // Additional Features double tap guard
    [U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

    // Mac Mode
    [U_TD_MAC] = ACTION_TAP_DANCE_FN(u_td_mac_fn),
    [U_TD_WIN] = ACTION_TAP_DANCE_FN(u_td_win_fn),

    // Paste Special
    [U_TD_PST] = ACTION_TAP_DANCE_FN(u_td_pst_sp_fn),

    // Screenshot
    [U_TD_PSCR] = ACTION_TAP_DANCE_FN(u_td_pscr_fn),

    // Em Dash
    [U_TD_EMDASH] = ACTION_TAP_DANCE_FN(u_td_emdash_fn),
};

// Define U_PST as paste special tap dance to work across all keymaps.
#define U_PST TD(U_TD_PST)

// CUSTOM KEYCODE HANDLING
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
            
        // Perform redo action for Mac (Cmd+Shift+Z)
        case U_RDO:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LCMD);
                    register_code(KC_LSFT);
                    tap_code(KC_Z);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LCMD);
                    return false;
                }
            }
            return true; // Else use default redo (Windows)
        
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
                    tap_code(keycode == U_BRWSR_BCK ? KC_WBAK : KC_WFWD); // Tap Browser Back for U_BRWSR_BCK or Browser Forward for U_BRWSR_FWD
                }
            }
            return false;

            // Search with U_SEARCH
            case U_SEARCH:
                if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LCMD); // Press and hold Left Command
                    tap_code(KC_SPC); // Tap Space
                    unregister_code(KC_LCMD); // Release Left Command
                } else {
                    register_code(KC_LGUI); // Press and hold Left GUI (Windows key)
                    tap_code(KC_S); // Tap S
                    unregister_code(KC_LGUI); // Release Left GUI
                }
            }
            return false;

        // Excel Shortcuts
        case U_XWIN:
        case U_XFRZ:
        case U_XIND:
        case U_XOUT:
        case U_XDECINC:
        case U_XDECDEC:
            if (record->event.pressed) {
                tap_code(KC_LALT);
                switch (keycode) {
                    case U_XWIN:
                        tap_code(KC_W);
                        tap_code(KC_N);
                        break;
                    case U_XFRZ:
                        tap_code(KC_W);
                        tap_code(KC_F);
                        tap_code(KC_F);
                        break;
                    case U_XIND:
                        tap_code(KC_H);
                        tap_code(KC_6);
                        break;
                    case U_XOUT:
                        tap_code(KC_H);
                        tap_code(KC_5);
                        break;
                    case U_XDECINC:
                        tap_code(KC_H);
                        tap_code(KC_0);
                        break;
                    case U_XDECDEC:
                        tap_code(KC_H);
                        tap_code(KC_9);
                        break;
                }
            }
            return false;

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
