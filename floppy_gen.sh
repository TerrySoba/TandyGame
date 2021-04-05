#!/bin/sh

if test -f boot_floppy/Dos3.3.img
then
    cp boot_floppy/Dos3.3.img tandy_game.img
else
    cp boot_floppy/FLOPPY.img tandy_game.img
fi

mcopy -i tandy_game.img release ::game
mcopy -o -i tandy_game.img boot_floppy/autoexec.bat ::AUTOEXEC.BAT
