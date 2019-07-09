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
		Image bg("vgabg.img");

		Animation guy("guy.ani");

		int y = 0;
		
		// TandyGfx gfx;
		VgaGfx gfx;
		gfx.setBackground(bg);
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
			
			gfx.drawImageTransparent(guy, 10 + y & 63, 10 + y & 63, 0);

			gfx.drawScreen();

			++y;
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
