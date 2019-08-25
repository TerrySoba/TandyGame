#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"
#include "version.h"
#include "physics.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "unit_test.h"

int main()
{
	try
	{
		// RadPlayer music("CELT.RAD");
		Keyboard keyboard;

		Image img("rgb.img");
		Image bg("way.img");

		Animation guy("guy.ani");
		guy.useTag(1);

		int y = 100;
		int x = 100;
		
		// TandyGfx gfx;
		VgaGfx gfx;
		gfx.setBackground(bg);
		
		int frames = 0;

		Physics physics;
		Actor actor;
		actor.rect.x = 10 << 4;
		actor.rect.y = 100 << 4;
		actor.rect.width = guy.width();
		actor.rect.height = guy.height();
		actor.dx = 0;// 30;
		actor.dy = 0;// -50;
		actor.jumpFrame = 1;
		int player = physics.addActor(actor);

		Rectangle ground(0, PIXEL_TO_SUBPIXEL(150), PIXEL_TO_SUBPIXEL(320), PIXEL_TO_SUBPIXEL(20));

		physics.addWall(ground);

		int16_t playerX;
		int16_t playerY;

		while (!s_keyEsc)
		{
			gfx.clear();
			gfx.drawImage(guy, x, y);
			// gfx.drawImageTransparent(guy, x, 100);

			physics.getActorPos(player, playerX, playerY);

			gfx.drawImageTransparent(guy, SUBPIXEL_TO_PIXEL(playerX), SUBPIXEL_TO_PIXEL(playerY));

			gfx.drawText("ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789#", 10, 10);
			gfx.drawText("What is your name, please?", 10, 20);
			gfx.drawText("That is a secret! Name: Unknown.", 10, 30);

			gfx.drawScreen();

			++frames;

			if (s_keyRight)
			{
				physics.setActorSpeed(player, 16, 0);
				++x;
			}
			
			if (s_keyLeft)
			{
				physics.setActorSpeed(player, -16, 0);
				--x;
			}
			if (s_keyUp) --y;
			if (s_keyDown) ++y;

			if (s_keyAlt) physics.startActorJump(player);


			if (frames % 4 == 0) guy.nextFrame();
			physics.calc();
		}

	}
	catch(const std::exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}

	printf("Build date: %s\nhttps://github.com/TerrySoba/TandyGame\n\nThanks for playing!\n", BUILD_DATE);

	return 0;
}
