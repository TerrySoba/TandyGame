#include <stdio.h>

#include "vgagfx.h"
#include "image.h"
#include "animation.h"

#include <stdio.h>
#include <iostream>

int main()
{
	try
	{
		Image img("rgb.img");
		Image bg("way.img");

		Animation guy("guy.ani");
		guy.useTag(1);

		int y = 0;
		
		// TandyGfx gfx;
		VgaGfx gfx;
		gfx.setBackground(bg);
		
		int frames = 0;

		for (;;)
		{
			// gfx.drawScreen();
			// gfx.vsync();
			gfx.clear();
			
			// gfx.drawImage(img, 10, 10);

			// gfx.drawImage(img, 10 + y & 63, 10 + y & 63);
			// gfx.drawImage(img, 10 + y & 63, 10 + y & 127);
			// gfx.drawImage(img, 10 + y & 127, 10 + y & 63);
			// gfx.drawImage(img, 10 + y & 127, 10 + y & 127);
			
			// gfx.drawImageTransparent(guy, 10 + y & 63, 10 + y & 63, 0);
			if (y > 300) y = 0;

			gfx.drawImage(guy, y, 79);
			gfx.drawImageTransparent(guy, y, 100);

			gfx.drawScreen();

			++frames;
			++y;

			if (frames % 4 == 0) guy.nextFrame();
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
