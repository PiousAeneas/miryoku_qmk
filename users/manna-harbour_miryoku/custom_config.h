// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

/*
    Custom Media Layer. Adds Browser, System, and Excel Shortcuts.
    1. CG_TOGG / CG_NORM toggles swaps Control and GUI, including for Paste, Copy, Cut, and Undo. Tap dance actions added in manna-harbour_miryoku.c
    2. U_TABF / U_TABB are WinMac shortcuts for toggling browser tabs.
*/

#define U_TABF LCTL(KC_TAB)
#define U_TABB LCTL(LSFT(KC_TAB))

#define MIRYOKU_LAYER_MEDIA \
TD(U_TD_BOOT),       TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_NA,               RGB_TOG,        RGB_MOD,       U_TABB,      U_TABF,     RGB_VAI,           \
KC_LGUI,             KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,               U_NU,           KC_MPRV,       KC_VOLD,     KC_VOLU,    KC_MNXT,           \
TD(U_TD_CG_NORM),    KC_ALGR,           TD(U_TD_U_FUN),    TD(U_TD_U_MEDIA),  TD(U_TD_CG_TOGG),   OU_AUTO,        U_NU,          U_NU,        U_NU,       U_NU,              \
U_NP,                U_NP,              U_NA,              U_NA,              U_NA,               KC_MSTP,        KC_MPLY,       KC_MUTE,     U_NP,       U_NP
