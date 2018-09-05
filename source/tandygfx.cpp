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


TandyGfx::TandyGfx()
{
    long int screenBytes = TANDY_SCREEN_W * TANDY_SCREEN_H / 2;

    m_screenBuffer = new char[screenBytes];

    memset(m_screenBuffer, 0x0, screenBytes);

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

static char far* getScreenLine(int line)
{
    return screen[line & 3] + LINE_BYTES * (line >> 2);
}

void TandyGfx::drawScreen()
{
    waitForVsync();
    for (int i = 0; i < m_dirtyRects.size(); ++i)
    {
        Rectangle& rect = m_dirtyRects[i];
        for (int y = rect.y; y < rect.y + rect.height; ++y)
        {
            memcpy(getScreenLine(y) + rect.x / 2 , m_screenBuffer + LINE_BYTES * y + rect.x / 2, rect.width / 2);
        }
    }

    m_dirtyRects.clear();
}

void TandyGfx::drawImage(const Image& image, int targetX, int targetY)
{
    char* imageData = image.data();
    const int imageLineBytes = image.width() / 2;

    Rectangle rect = {
        targetX, targetY,
        image.width(), image.height()};
    m_dirtyRects.push_back(rect);

    for (int y = 0; y < image.height(); ++y)
    {
        memcpy(m_screenBuffer + LINE_BYTES * (targetY + y) + targetX / 2, imageData + imageLineBytes * y, imageLineBytes);
    }
}
