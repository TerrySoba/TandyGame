objects1 = main.o vgagfx.o image.o animation.o keyboard.o rad_player.o rad_player_asm.o timer.o physics.o 
objects_test = test_main.o unit_test.o physics.o physics_test.o
# coptions = -0 -ml -xs -onatx -oh -oi+ -ei -zp8 -fpi87
# coptions = -0 -ml -xs -onatx -oh -ol -ol+ -ox
coptions = -ml -oneatx -xs -zp4 -0

dest     = game.exe
test     = gametest.exe

BUILD_DATE=`date -R`

all: $(dest) $(test)

$(dest): _$(dest)
	upx -9 --ultra-brute -f --8086 -o$(dest) _$(dest)

main.o: vgagfx.h image.h animation.h keyboard.h rad_player.h version.h physics.h
vgagfx.o: image_base.h animation.h rectangle.h
image.o: image_base.h
animation.o: image.h
keyboard.o: 
rad_player.o: timer.h
timer.o: 
physics.o: rectangle.h
test_main.o: unit_test.h
unit_test.o: unit_test.h
physics_test.o: unit_test.h

%.o : %.cpp
	wpp $(coptions) $<

rad_player_asm.o: 3rd_party/rad_tracker/c_interface.asm 3rd_party/rad_tracker/player_nasm.asm
	nasm -f obj -i 3rd_party/rad_tracker  3rd_party/rad_tracker/c_interface.asm -o rad_player_asm.o -DRAD_NO_DEFAULT_PLAYER

FORCE:

version.h: FORCE
	echo \#define BUILD_DATE \"$(BUILD_DATE)\" > version.h

_$(dest): $(objects1)
	echo "NAME   $@" > game.lnk
	echo "SYSTEM DOS" >> game.lnk
	echo "FILE   {$(objects1)}" >> game.lnk
	wlink  @game.lnk

$(test): $(objects_test)
	echo "NAME   $@" > gametest.lnk
	echo "SYSTEM DOS" >> gametest.lnk
	echo "FILE   {$(objects_test)}" >> gametest.lnk
	wlink  @gametest.lnk