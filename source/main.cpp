#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <conio.h>


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


static void __interrupt __far customTimerInterrupt()
{
	radPlayMusic();

	// signal interrupt done
	__asm{
    	"mov al,20H"
    	"out 20H,al"
	}
}

#define TIMER_INTERRUPT 0x08

void timerInit(void)
{
    // The clock we're dealing with here runs at 1.193182mhz, so we
    // just divide 1.193182 by the number of triggers we want per
    // second to get our divisor.
    uint32_t c = 1193181 / (uint32_t)50;

    _dos_setvect(TIMER_INTERRUPT, customTimerInterrupt);

    __asm{ "cli" }

    // There's a ton of options encoded into this one byte I'm going
    // to send to the PIT here so...

    // 0x34 = 0011 0100 in binary.

    // 00  = Select counter 0 (counter divisor)
    // 11  = Command to read/write counter bits (low byte, then high
    //       byte, in sequence).
    // 010 = Mode 2 - rate generator.
    // 0   = Binary counter 16 bits (instead of BCD counter).

    outp(0x43, 0x34);

    // Set divisor low byte.
    outp(0x40, (uint8_t)(c & 0xff));

    // Set divisor high byte.
    outp(0x40, (uint8_t)((c >> 8) & 0xff));

    __asm { "sti" }
}


int main()
{
	void* radModule = radLoadModule("RASTER.RAD");
	if (!radModule)
	{
		printf("Error loading module.");
		return 1;
	}

	timerInit();
	initKeyboard();

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

	free(radModule);
	return 0;
}
