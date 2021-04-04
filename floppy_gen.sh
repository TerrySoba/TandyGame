#!/bin/sh

cp boot_floppy/FLOPPY.img tandy_game.img
mcopy -i tandy_game.img release ::game
mcopy -o -i tandy_game.img boot_floppy/autoexec.bat ::AUTOEXEC.BAT
