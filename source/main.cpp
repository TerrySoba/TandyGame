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
#include "joystick.h"

#include "exception.h"

#include <stdio.h>
#include <stdlib.h>

LevelNumber parseCommandline(int argc, char* argv[])
{
	LevelNumber level = {1, 1};

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("--level", argv[i]) == 0 && i+2 < argc)
		{
			int16_t levelX, levelY;
			sscanf(argv[i+1], "%d", &levelX);
			sscanf(argv[i+2], "%d", &levelY);
			i+=2;

			level.x = levelX;
			level.y = levelY;
		}		
	}

	return level;
}


int main(int argc, char* argv[])
{
	LevelNumber level = parseCommandline(argc, argv);
	calibrateJoystick();
	
	try
	{
		RadPlayer music("CELT.RAD");
		Keyboard keyboard;

		shared_ptr<ImageBase> tiles(new TgaImage("tiles.tga"));
		
		shared_ptr<Animation> enemy(new Animation("enemy.jsn", "enemy.tga"));
		shared_ptr<Animation> guffin(new Animation("guffin.jsn", "guffin.tga"));
		shared_ptr<Animation> guy(new Animation("guy.jsn", "guy.tga"));

		shared_ptr<VgaGfx> gfx(new VgaGfx);

		GameAnimations animations = {guy, enemy, guffin};

		Game game(gfx, tiles, animations, "%02x%02x", level);

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
