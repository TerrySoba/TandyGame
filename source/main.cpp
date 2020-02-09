#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"
#include "tga_image.h"
#include "level.h"
#include "game.h"
#include "version.h"
#include "shared_ptr.h"

#include "exception.h"

#include <stdio.h>
#include <stdlib.h>



int main()
{
	try
	{
		RadPlayer music("CELT.RAD");
		Keyboard keyboard;

		shared_ptr<ImageBase> tiles(new TgaImage("tiles.tga"));
		
		shared_ptr<Animation> guy(new Animation("guy.jsn", "guy.tga"));
		shared_ptr<VgaGfx> gfx(new VgaGfx);

		Game game(gfx, tiles, guy, "l%02d");

		game.loadLevel(1);

		while (!s_keyEsc)
    	{	
			game.drawFrame();
		}
		
	}
	catch(const Exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}
	catch(...)
	{
		fprintf(stderr, "Unknown exception.");
		return 1;
	}

	printf("Build date: %s\nhttps://github.com/TerrySoba/TandyGame\n\nThanks for playing!\n", BUILD_DATE);

	return 0;
}
