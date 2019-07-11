objects1 = main.o vgagfx.o image.o animation.o keyboard.o
# coptions = -0 -ml -xs -onatx -oh -oi+ -ei -zp8 -fpi87
# coptions = -0 -ml -xs -onatx -oh -ol -ol+ -ox
coptions = -ml -oneatx -xs -zp4 -0

dest     = game.exe

all: $(dest)

main.o:
  wpp $(coptions) main.cpp

vgagfx.o:
  wpp $(coptions) vgagfx.cpp
  # wdis vgagfx.o -s -l

image.o:
  wpp $(coptions) image.cpp

animation.o:
  wpp $(coptions) animation.cpp

keyboard.o:
  wpp $(coptions) keyboard.cpp

$(dest): $(objects1)
  %write game.lnk NAME   $@
  %write game.lnk SYSTEM DOS
  %write game.lnk FILE   {$(objects1)}
  wlink  @game.lnk

