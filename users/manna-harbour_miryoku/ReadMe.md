![Atreus](https://github.com/PiousAeneas/miryoku_qmk/assets/53350783/afecf96d-f86e-4db9-8910-e4a7cc03cdb3)

The following files were edited compared with the original repo:
- manna-harbour_miryoku.c
- custom_config.h
- custom_rules.mk

Edited `manna-harbour_miryoku.c` to add the following custom actions:
1. Mac Mode via U_TD_MAC and U_TD_WIN.
2. OS-Specific Tab Navigation via U_TABB and U_TABF.
3. OS-Specific Browser Navigation via U_BRWSR_BCK and U_BRWSR_FWD.
4. OS-Specific Spotlight Search via U_SEARCH.
5. Set Windows as default Clipboard by overriding definitions from manna-harbour_miryoku.h since Mac Mode swaps Control and GUI for Paste, Copy, Cut, and Undo.
6. OS-Specific Redo via process_record_user intercept for U_RDO
7. OS-Specific Paste and Paste Special tap dance to replace U_PST
8. Excel Shortcuts: New Window, Freeze, Indent, Outdent, Add Decimal, Remove Decimal
9. OS-Specific Screenshot tap dance using U_TD_PSCR.

Edited `custom_config.h` primarily to add a custom MIRYOKU_LAYER_MEDIA and to aedit Print Screen on MIRYOKU_LAYER_FUN. Also added optimizations to reduce firmware size.

Edited `custom_rules.mk` to add optimizations to reduce firmware size.
