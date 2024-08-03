![Custom Miryoku Key Map](https://github.com/PiousAeneas/miryoku_qmk/blob/miryoku/users/manna-harbour_miryoku/keyboard-layout-custom-miryoku.png)
Edit the layout by dragging the [Keyboard Layout Editor JSON](https://github.com/PiousAeneas/miryoku_qmk/blob/miryoku/users/manna-harbour_miryoku/keyboard-layout-custom-miryoku.json) into [http://www.keyboard-layout-editor.com/#/](http://www.keyboard-layout-editor.com/#/)

View and edit the full layout with [this](https://github.com/PiousAeneas/miryoku_qmk/blob/miryoku/users/manna-harbour_miryoku/keyboard-layout-custom-miryoku-full.png) PNG and [this](https://github.com/PiousAeneas/miryoku_qmk/blob/miryoku/users/manna-harbour_miryoku/keyboard-layout-custom-miryoku-full.json) JSON.

The following files were edited compared with the original repo:
- manna-harbour_miryoku.c
- custom_config.h
- custom_rules.mk

Firmware can be compiled using "Build Inputs" Actions workflow with all settings set to "default" since Clipboard is overriden to default to Win. Alphas default to Colemak-DH with "Extra" layer defaulting to QWERTY.

Edited `manna-harbour_miryoku.c` to add the following custom actions:
1. Mac Mode via U_TD_MAC and U_TD_WIN.
2. OS-Specific Tab Navigation via U_TABB and U_TABF.
3. OS-Specific Browser Navigation via U_BRWSR_BCK and U_BRWSR_FWD.
4. OS-Specific Spotlight Search via U_SEARCH.
5. Set Windows as default Clipboard by overriding definitions from `manna-harbour_miryoku.h` since Mac Mode swaps Control and GUI for Paste, Copy, Cut, and Undo.
6. OS-Specific Redo via process_record_user intercept for U_RDO (affects Nav, Mouse, and Button Layers).
7. OS-Specific Paste and Paste Special tap dance to replace U_PST (affects Nav, Mouse, and Button Layers).
8. OS-Specific em dash symbol via U_MDASH (for custom Sym layer).
9. Excel Shortcuts: New Window, Freeze, Indent, Outdent, Add Decimal, Remove Decimal
10. OS-Specific Screenshot tap dance using U_TD_PSCR.

Edited `custom_config.h` to make the following changes:
1. MIRYOKU_LAYER_MEDIA becomes "System" with Mac Mode toggles, Tab Navigation, Browser Navigation, Spotlight Search, and Excel Shortcuts.
2. MIRYOKU_LAYER_FUN gets a screen shot tap dance action instead of PrtScn i.e. `KC_PSCR`.
3. MIRYOKU_LAYER_SYM gets a custom emdash keycode to replace underscore i.e. `KC_UNDS`.
4. MIRYOKU_LAYER_MOUSE gets mouse acceleration keys.
5. Added `#define CAPS_WORD_INVERT_ON_SHIFT` to modify Caps Word to invert on Shift. This required copying over all the changes from [#20092](https://github.com/qmk/qmk_firmware/commit/ae63c0f509fae71270fb5885d504ee26cbad95ff) because of [#287](https://github.com/manna-harbour/miryoku/discussions/287).
6. Added `#define PERMISSIVE_HOLD` to turn on permissive hold globally for use with e.g. layer tap for Caps Word, Shift keys, Ctrl+Del.
7. Added `#define MK_COMBINED` to turn on combined mouse mode to add speed toggles to Accelerated mode.
8. Added optimizations to reduce firmware size.

Edited `custom_rules.mk` to add optimizations to reduce firmware size.
