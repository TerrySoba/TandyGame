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

	gfx.setBackgroundImage(bg);

	for (;;)
	{
		++x;
		if (x > 150) x = 0;

		gfx.drawBackground();
		gfx.drawImage(img, x, 150);
		gfx.drawImage(img, 150, x);
	    
	}
	
	getchar();
	return 0;
}
