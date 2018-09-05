#include <stdio.h>

#include "tandygfx.h"
#include "image.h"

#include <stdio.h>
#include <vector>

int main()
{
	TandyGfx gfx;

	// Image img(32, 16, 0xff);
	Image bg("out.img");
	Image img("rgb.img");

	int x = 0;

	gfx.drawImage(bg, 0, 0);

	for (;;)
	{
		++x;
		if (x > 100) x = 0;
		gfx.drawImage(img, x, 100);
		gfx.drawImage(img, 100, x);
	    gfx.drawScreen();
	}
	
	getchar();
	return 0;
}
