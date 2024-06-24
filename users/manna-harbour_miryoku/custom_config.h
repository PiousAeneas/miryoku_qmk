// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

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
