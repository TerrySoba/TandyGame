#include "vgagfx.h"
#include "image.h"
#include "animation.h"
#include "keyboard.h"
#include "rad_player.h"
#include "tga_image.h"
#include "level.h"
#include "game.h"
#include "version.h"
#include "shared_ptr.h"
#include "joystick.h"

#include "exception.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


void disableCursor()
{
	__asm {
		push ax
		push cx

		mov ah, 01h
        mov ch, 03Fh // (bits 0-7 unused, bit 5 disables cursor, bits 0-4 control cursor shape)

		int 10h

		pop cx
		pop ax
	}
}

void setCursor(uint8_t x, uint8_t y)
{
	__asm {
		push ax
		push bx
		push dx


		mov ah, 02h
		mov bh, 00h
		mov dh, x
		mov dl, y

		int 10h

		pop dx
		pop bx
		pop ax
	}
}


void calibrateJoystick()
{
	JoystickStatus s = readJoystick();

	uint16_t centerX = s.x1;
	uint16_t centerY = s.y1;

	uint16_t minX = s.x1;
	uint16_t minY = s.y1;
	uint16_t maxX = s.x1;
	uint16_t maxY = s.y1;

	disableCursor();

	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	setCursor(0,0);
	puts("Calibrate Joystick");
	setCursor(1,0);
	puts("ออออออออออออออออออ");
	setCursor(4,0);
	puts("Rotate joystick, then center joystick and press joystick button.");
	setCursor(19,0);
	puts("Press \"Q\" on keyboard to disable joystick support.");
	setCursor(9,6);
	puts("ษออออออหออออออหออออออป");
	setCursor(10,6);
	puts("บ      บ      บ      บ");
	setCursor(11,6);
	puts("ฬออออออฮออออออฮออออออน");
	setCursor(12,6);
	puts("บ      บ  ÛÛ  บ      บ");
	setCursor(13,6);
	puts("ฬออออออฮออออออฮออออออน");
	setCursor(14,6);
	puts("บ      บ      บ      บ");
	setCursor(15,6);
	puts("ศออออออสออออออสออออออผ");
	bool done = false;

	while (!done)
	{
		if (kbhit() && getch() == 'q')
		{
			done = true;
		}

		JoystickStatus s = readJoystick();

		char buf[8];

		setCursor(6,3);
		sprintf(buf, "x:%04hd", s.x1);
		puts(buf);
		setCursor(7,3);
		sprintf(buf, "y:%04hd", s.y1);
		puts(buf);
	}
}


int main()
{
	JoystickStatus s = readJoystick();

	bool joystickAvailable = true;
	if (s.buttons == 0xff || (s.x1 == 0 && s.y1 == 0))
	{
		joystickAvailable = false;
	}

	// for (;;)
	// {
	// 	s = readJoystick();
	// 	printf("buttons:%02hhx x1:%04hx y1:%04hx x2:%04hx y2:%04hx\n", s.buttons, s.x1, s.y1, s.x2, s.y2);
	// }

	if (joystickAvailable) 
	{
		calibrateJoystick();
	}
	
	


	try
	{
		RadPlayer music("CELT.RAD");
		Keyboard keyboard;

		shared_ptr<ImageBase> tiles(new TgaImage("tiles.tga"));
		
		shared_ptr<Animation> guy(new Animation("guy.jsn", "guy.tga"));
		shared_ptr<VgaGfx> gfx(new VgaGfx);

		Game game(gfx, tiles, guy, "%02x%02x");

		LevelNumber levelNumber = {1,1};
		game.loadLevel(levelNumber, UseSpawnPoint::YES);

		while (!s_keyEsc)
    	{	
			
			game.drawFrame();
		}
		
	}
	catch(const Exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}
	catch(...)
	{
		fprintf(stderr, "Unknown exception.");
		return 1;
	}

	printf("\033[2J");           // clear screen
	printf("Build date: %s\nhttps://github.com/TerrySoba/TandyGame\n\nThanks for playing!\n", BUILD_DATE);

	return 0;
}
