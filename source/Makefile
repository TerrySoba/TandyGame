objects1 = main.o tandygfx.o
coptions = -0 -ml
dest     = game.exe

all: $(dest)

main.o:
  wpp $(coptions) main.cpp

tandygfx.o:
  wpp $(coptions) tandygfx.cpp

$(dest): $(objects1)
  %write game.lnk NAME   $@
  %write game.lnk SYSTEM DOS
  %write game.lnk FILE   {$(objects1)}
  wlink  @game.lnk
