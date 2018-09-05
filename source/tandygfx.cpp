#include "tandygfx.h"

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>

#define inport(px) inpw(px)
#define inportb(px) inp(px)

#define TANDY_SCREEN_W 320L
#define TANDY_SCREEN_H 200L

static void videoInit()
{
    __asm{
        mov ah, 00h
        mov al, 09h
        int 10h
       }
}

void loadImage(char* out)
{
    FILE* fp = fopen("out.img", "rb");

	if (!fp)
	{
		printf("Could not open file \"%s\".\n", "out.img");
        return;
	}

	short int width, height;

    fread(&width, 2, 1, fp);
    fread(&height, 2, 1, fp);

    fread(out, 1, width * height / 2, fp);

	fclose(fp);
}


TandyGfx::TandyGfx()
{
    long int screenBytes = TANDY_SCREEN_W * TANDY_SCREEN_H / 2;

    m_screenBuffer = new char[screenBytes];

    memset(m_screenBuffer, 0x17, screenBytes);

    loadImage(m_screenBuffer);

    videoInit();
}

TandyGfx::~TandyGfx()
{
    delete[] m_screenBuffer;   
}

#define CGA_STATUS_REG 0x03DA
#define VERTICAL_RETRACE_BIT 0x08

#define waitForVsync() while (! (inportb(CGA_STATUS_REG) & VERTICAL_RETRACE_BIT))

#define LINE_BYTES 160L

static char far* screen[4] = {(char far*)(0xB8000000L), (char far*)(0xB8002000L), (char far*)(0xB8004000L), (char far*)(0xB8006000L)};

void TandyGfx::drawScreen()
{
    waitForVsync();    
    for (int i = 0; i < 4; ++i)
    {
        for (int line = 0; line < TANDY_SCREEN_H / 4; ++line)
        {
            memcpy(screen[i] + line * LINE_BYTES, m_screenBuffer + (line * 4 + i) * LINE_BYTES, LINE_BYTES);
        }
    }
}

void TandyGfx::drawImage(const Image& image, int targetX, int targetY)
{
    char* imageData = image.data();
    const int imageLineBytes = image.width() / 2;
    for (int y = 0; y < image.height(); ++y)
    {
        memcpy(m_screenBuffer + LINE_BYTES * (targetY + y) + targetX / 2, imageData + imageLineBytes * y, imageLineBytes);
    }
}
