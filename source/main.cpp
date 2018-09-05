#include <stdio.h>

#include "tandygfx.h"
#include "image.h"

#include <stdio.h>
#include <vector>

int main()
{
	TandyGfx gfx;

	// Image img(32, 16, 0xff);
	Image img("rgb.img");

	for (;;)
	{
		gfx.drawImage(img, 100, 100);
	    gfx.drawScreen();
	}
	
	getchar();
	return 0;
}
