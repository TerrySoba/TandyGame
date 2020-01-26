#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"
#include "version.h"
#include "physics.h"
#include "tga_image.h"
#include "level.h"

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

		// Image img("rgb.img");
		// Image bg("way.img");
		shared_ptr<ImageBase> tiles(new TgaImage("tiles.tga"));

		Level level("l01_bg.csv", "l01_col.csv", tiles, 16, 16);

		// return 0;

		// Animation guy("guy.ani");
		Animation guy("guy.jsn", "guy.tga");
		// guy.useTag(1);
		guy.useTag("Loop");

		int y = 100;
		int x = 100;
		
		// TandyGfx gfx;
		VgaGfx gfx;

		gfx.drawBackground(level, 0, 0);

		// gfx.setBackground(*tiles);
		
		int frames = 0;

		Physics physics;
		Actor actor;
		actor.rect.x = PIXEL_TO_SUBPIXEL(150);
		actor.rect.y = PIXEL_TO_SUBPIXEL(100);
		actor.rect.width = PIXEL_TO_SUBPIXEL(guy.width());
		actor.rect.height = PIXEL_TO_SUBPIXEL(guy.height());
		actor.dx = 0;// 30;
		actor.dy = 0;// -50;
		actor.jumpFrame = 1;
		int player = physics.addActor(actor);

		// Rectangle ground(0, PIXEL_TO_SUBPIXEL(80), PIXEL_TO_SUBPIXEL(320), PIXEL_TO_SUBPIXEL(20));

		// physics.addWall(ground);
		physics.setWalls(level.getWalls());

		int16_t playerX;
		int16_t playerY;

		gfx.clear();

		char buf[255];

		snprintf(buf, 255, "Build date: %s", BUILD_DATE);


		gfx.drawText(buf, 50, 193);

		// gfx.drawText("ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789#", 10, 10);
		// gfx.drawText("What is your name, please?", 10, 20);
		// gfx.drawText("That is a secret! Name: Unknown.", 10, 30);

		while (!s_keyEsc)
		{
			gfx.clear();
			// gfx.drawImage(guy, x, y);
			// gfx.drawImageTransparent(guy, x, 100);

			physics.getActorPos(player, playerX, playerY);

			gfx.draw(guy, SUBPIXEL_TO_PIXEL(playerX), SUBPIXEL_TO_PIXEL(playerY));

		

			gfx.drawScreen();

			++frames;

			if (s_keyRight)
			{
				physics.setActorSpeedX(player, 16);
				++x;
			}
			
			if (s_keyLeft)
			{
				physics.setActorSpeedX(player, -16);
				--x;
			}
			if (s_keyUp) --y;
			if (s_keyDown) ++y;

			if (s_keyAlt)
			{
				// physics.setActorSpeedY(player, -32);
				physics.startActorJump(player);
			}


			if (frames % 4 == 0) guy.nextFrame();
			physics.calc();
		}

	}
	catch(const Exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}
	catch(...)
	{
		fprintf(stderr, "Unknown excxeption.");
		return 1;
	}

	printf("Build date: %s\nhttps://github.com/TerrySoba/TandyGame\n\nThanks for playing!\n", BUILD_DATE);

	return 0;
}
