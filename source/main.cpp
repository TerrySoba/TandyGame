#include <stdio.h>

#include "tandygfx.h"
#include "image.h"

#include <stdio.h>

int main()
{
	TandyGfx gfx;

	Image img("rgb.img");
	Image bg("out.img");

	int y = 0;


	gfx.setBackground(bg);
	for (;;)
	{
		// gfx.drawScreen();
		// gfx.vsync();
		gfx.clear();
		
		gfx.drawImage(img, 10 + y & 63, 10 + y & 63);
		gfx.drawImage(img, 10 + y & 63, 10 + y & 127);
		gfx.drawImage(img, 10 + y & 127, 10 + y & 63);
		gfx.drawImage(img, 10 + y & 127, 10 + y & 127);
		
		gfx.drawScreen();

		++y;
	}


	// Image bg("out.img");
	

	

	// gfx.setBackground(bg);

	// for (;;)
	// {
	// 	++x;
	// 	if (x > 100) x = 0;
	// 	gfx.drawImage(img, x, 100);
	// 	gfx.drawImage(img, 100, x);
	//     gfx.drawScreen();
	// }
	
	getchar();
	return 0;
}
