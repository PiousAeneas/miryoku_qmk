// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#define CAPS_WORD_INVERT_ON_SHIFT // Holding Shift continues Caps Word but inverts the shift state
#define PERMISSIVE_HOLD // Selects the hold action when another key is pressed and then released while the dual-role key is held down, even if within the tapping term

/*
    Custom Media Layer. Adds Browser, System, and Excel Shortcuts.
    1. U_TD_MAC / U_TD_WIN on double tap turns on Mac Mode which swaps Control and GUI including for Paste, Copy, Cut, and Undo (but not Redo or Paste Special).
    2. U_TABB / U_TABF are Win/Mac compatible shortcuts for toggling back / forward through browser tabs.
    3. U_BRWSR_BCK / U_BRWSR_FWD are Win/Mac compatible for navigating backwards / forwards in a browser.
    4. U_SEARCH enables Spotlight search for both Windows and Mac.
    5. Excel Shortcuts: New Window, Freeze, Indent, Outdent, Add Decimal, Remove Decimal
*/

#define MIRYOKU_LAYER_MEDIA \
TD(U_TD_BOOT),    TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_NA,              U_XWIN,         U_BRWSR_BCK,      U_TABB,      U_TABF,      U_BRWSR_FWD,       \
KC_LGUI,          KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              U_SEARCH,       KC_MPRV,          KC_VOLD,     KC_VOLU,     KC_MNXT,           \
TD(U_TD_WIN),     KC_ALGR,           TD(U_TD_U_FUN),    TD(U_TD_U_MEDIA),  TD(U_TD_MAC),      U_XFRZ,         U_XOUT,           U_XDECDEC,   U_XDECINC,   U_XIND,            \
U_NP,             U_NP,              U_NA,              U_NA,              U_NA,              KC_MSTP,        KC_MPLY,          KC_MUTE,     U_NP,        U_NP

// Replaced KC_PSCR with TD(U_TD_PSCR)
#define MIRYOKU_LAYER_FUN \
KC_F12,            KC_F7,             KC_F8,             KC_F9,             TD(U_TD_PSCR),     U_NA,              TD(U_TD_U_BASE),   TD(U_TD_U_EXTRA),  TD(U_TD_U_TAP),    TD(U_TD_BOOT),     \
KC_F11,            KC_F4,             KC_F5,             KC_F6,             KC_SCRL,           U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
KC_F10,            KC_F1,             KC_F2,             KC_F3,             KC_PAUS,           U_NA,              TD(U_TD_U_FUN),    TD(U_TD_U_MEDIA),  KC_ALGR,           U_NA,              \
U_NP,              U_NP,              KC_APP,            KC_SPC,            KC_TAB,            U_NA,              U_NA,              U_NA,              U_NP,              U_NP

// Replaced KC_MINS with TD(U_TD_EMDASH)
#define MIRYOKU_LAYER_NUM \
KC_LBRC,           KC_7,              KC_8,              KC_9,              KC_RBRC,           U_NA,              TD(U_TD_U_BASE),   TD(U_TD_U_EXTRA),  TD(U_TD_U_TAP),    TD(U_TD_BOOT),     \
KC_SCLN,           KC_4,              KC_5,              KC_6,              KC_EQL,            U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
KC_GRV,            KC_1,              KC_2,              KC_3,              KC_BSLS,           U_NA,              TD(U_TD_U_NUM),    TD(U_TD_U_NAV),    KC_ALGR,           U_NA,              \
U_NP,              U_NP,              KC_DOT,            KC_0,              TD(U_TD_EMDASH),   U_NA,              U_NA,              U_NA,              U_NP,              U_NP

// Optimizations to reduce firmware size: https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
#define NO_ACTION_MACRO     // Required for LTO. Disables deprecated action_get_macro method and M(id) keycodes.
#define NO_ACTION_FUNCTION  // Required for LTO. Disables deprecated action_function method and AF(id) keycodes.
#define DISABLE_LEADER
