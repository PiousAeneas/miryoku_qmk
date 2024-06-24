# Copyright 2019 Manna Harbour
# https://github.com/manna-harbour/miryoku

# Firmware too large: https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
EXTRAFLAGS      += -flto
COMMAND_ENABLE   = no # Commands for debug and configuration
UNICODE_ENABLE   = no# Unicode
