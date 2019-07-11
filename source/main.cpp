#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"

#include <stdio.h>
// #include <iostream>
// #include <iomanip>

// char binBuffer[9];

// char* toBinary(uint8_t val)
// {
// 	binBuffer[8] = 0;

// 	for (int i = 0; i < 8; ++i)
// 		if ((1 << i) & val) binBuffer[i] = '1';
// 		else binBuffer[i] = '0';

// 	return binBuffer;
// }

int main()
{
	initKeyboard();
	// for (;;)
	// {
		
	// 	// std::cout << "code: 0x" << std::hex << std::setw(2) << (int)s_scancode << " 0b" << toBinary(s_scancode) << std::endl;

	// 	if (s_keyLeft) printf("left ");
	// 	printf("\n");
	// }
	// 	if (s_keyRight) std::cout << "right ";
	// 	if (s_keyUp) std::cout << "up ";
	// 	if (s_keyDown) std::cout << "down ";
	// 	if (s_keyAlt) std::cout << "alt ";
	// 	if (s_keyCtrl) std::cout << "ctrl ";
	// 	if (s_keySpace) std::cout << "space ";
	// 	if (s_keyEsc) std::cout << "esc ";
	// 	std::cout << std::endl;
	// }


	try
	{
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
			// if (y > 300) y = 0;

			gfx.drawImage(guy, x, y);
			gfx.drawImageTransparent(guy, x, 100);

			gfx.drawScreen();

			++frames;
			// ++y;

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
	return 0;
}
