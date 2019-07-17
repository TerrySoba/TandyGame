objects1 = main.o vgagfx.o image.o animation.o keyboard.o rad_player.o rad_player_asm.o timer.o
# coptions = -0 -ml -xs -onatx -oh -oi+ -ei -zp8 -fpi87
# coptions = -0 -ml -xs -onatx -oh -ol -ol+ -ox
coptions = -ml -oneatx -xs -zp4 -0

dest     = game.exe

all: $(dest)

main.o: 
  wpp $(coptions) main.cpp

timer.o: 
  wpp $(coptions) timer.cpp

vgagfx.o:
  wpp $(coptions) vgagfx.cpp
  # wdis vgagfx.o -s -l

image.o:
  wpp $(coptions) image.cpp

animation.o:
  wpp $(coptions) animation.cpp

keyboard.o:
  wpp $(coptions) keyboard.cpp

rad_player.o:
  wpp $(coptions) rad_player.cpp

rad_player_asm.o:
  nasm -f obj -i 3rd_party/rad_tracker  3rd_party/rad_tracker/c_interface.asm -o rad_player_asm.o -DRAD_NO_DEFAULT_PLAYER

$(dest): $(objects1) 
  %write game.lnk NAME   $@
  %write game.lnk SYSTEM DOS
  %write game.lnk FILE   {$(objects1)}
  wlink  @game.lnk
