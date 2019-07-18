#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>


int main()
{
	try
	{
		RadPlayer player("RASTER.RAD");
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

		while (!s_keyEsc)
		{
			gfx.clear();
			gfx.drawImage(guy, x, y);
			gfx.drawImageTransparent(guy, x, 100);

			gfx.drawScreen();

			++frames;

			if (s_keyRight) ++x;
			if (s_keyLeft) --x;
			if (s_keyUp) --y;
			if (s_keyDown) ++y;

			if (frames % 4 == 0) guy.nextFrame();
		}

	}
	catch(const std::exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}

	printf("Build date: %s\nGit revision: %s\nhttps://github.com/TerrySoba/TandyGame\n\nThanks for playing!\n", BUILD_DATE, GIT_HASH);

	return 0;
}
