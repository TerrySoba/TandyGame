#include "vgagfx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include "font_data.h"

#define inport(px) inpw(px)
#define inportb(px) inp(px)

#define SCREEN_W 320L
#define SCREEN_H 200L

extern void videoInit(uint8_t mode);
#pragma aux videoInit =    \
    "mov ah, 0"            \
    "int 10h"              \
    modify [ax]            \
    parm [al];

extern uint8_t getCurrentVideoMode();
#pragma aux getCurrentVideoMode =    \
    "mov ax, 0fh"          \
    "int 10h"              \
    modify [ax]            \
    value [al];

int compareRectangles(const void* a, const void* b) {
   return ( ((Rectangle*)a)->y > ((Rectangle*)b)->y );
}


void sortRects(std::vector<Rectangle>& rects)
{
    qsort(&rects[0], rects.size(), sizeof(Rectangle), compareRectangles);
}


VgaGfx::VgaGfx()
{
    long int screenBytes = SCREEN_W * SCREEN_H;

    m_backgroundImage = new char[screenBytes];
    m_screenBuffer = new char[screenBytes];
    
    memset(m_backgroundImage, 0xff, screenBytes);
    memset(m_screenBuffer, 0x0, screenBytes);

    Rectangle rect(0, 0, SCREEN_W, SCREEN_H);
    m_dirtyRects.push_back(rect);

    m_oldVideoMode = getCurrentVideoMode();
    videoInit(0x13);
}

VgaGfx::~VgaGfx()
{
    delete[] m_screenBuffer;
    delete[] m_backgroundImage;

    // videoInit(m_oldVideoMode);
    videoInit(0x03);
}

#define CGA_STATUS_REG 0x03DA
#define VERTICAL_RETRACE_BIT 0x08

#define waitForVsync() \
    while (inportb(CGA_STATUS_REG) & VERTICAL_RETRACE_BIT); \
    while (!(inportb(CGA_STATUS_REG) & VERTICAL_RETRACE_BIT))

#define LINE_BYTES 320L

static char far* screen = (char far*)(0xA0000000L);

#define getScreenLine(line) (screen + LINE_BYTES * (line))
#define getBackBufferLine(line) (m_screenBuffer + LINE_BYTES * (line))
#define getBackgroundImageLine(line) (m_backgroundImage + LINE_BYTES * (line))

void VgaGfx::vsync()
{
    waitForVsync();
}

#define lower(x) (x & 0x0f)
#define upper(x) ((x & 0xf0) >> 4)

#define set_lower(x, val) ((x & 0xf0) | val)
#define set_upper(x, val) ((x & 0x0f) | (val << 4))

class MemoryImage : public ImageBase
{
public:
    MemoryImage(int16_t width, int16_t height, char* data) :
        m_width(width), m_height(height), m_data(data)
    {}

    int16_t width() const { return m_width; }
	int16_t height() const { return m_height; }
	char* data() const { return m_data; }

private:
    int16_t m_width;
    int16_t m_height;
    char* m_data;

};

void VgaGfx::draw(const Drawable& image, int16_t x, int16_t y)
{
    Rectangle rect(
        x, y,
        image.width(), image.height());
    m_dirtyRects.push_back(rect);
    m_undrawnRects.push_back(rect);

    MemoryImage img(SCREEN_W, SCREEN_H, m_screenBuffer);

    image.draw(img, x, y);
}


void VgaGfx::drawImage(const ImageBase& image, int16_t targetX, int16_t targetY)
{
    const char* imageData = image.data();
    const int imageLineBytes = image.width();

    Rectangle rect(
        targetX, targetY,
        image.width(), image.height());
    m_dirtyRects.push_back(rect);
    m_undrawnRects.push_back(rect);

    for (int y = 0; y < image.height(); ++y)
    {
        memcpy(getBackBufferLine(y + targetY) + targetX, imageData + imageLineBytes * y, imageLineBytes);
    }
}

void VgaGfx::drawImageTransparent(const ImageBase& image, int16_t targetX, int16_t targetY, uint8_t transparentColor)
{
    const char* imageData = image.data();
    const int imageLineBytes = image.width();

    Rectangle rect(
        targetX, targetY,
        image.width(), image.height());
    m_dirtyRects.push_back(rect);
    m_undrawnRects.push_back(rect); 

    for (int y = 0; y < image.height(); ++y)
    {
        char* dest = getBackBufferLine(y + targetY) + targetX;
        const char* src  = imageData + imageLineBytes * y;

        for (uint16_t x = 0; x < image.width(); ++x)
        {
            if (*src != transparentColor)
            {
                *dest++ = *src++; 
            }
            else
            {
                ++dest;
                ++src;
            }
        }
    }
}

void VgaGfx::clear()
{
    for (int i = 0; i < m_dirtyRects.size(); ++i)
    {
        m_undrawnRects.push_back(m_dirtyRects[i]);
        const Rectangle& rect = m_dirtyRects[i];
        for (int y = 0; y < rect.height; ++y)
        {
            // memcpy(m_screenBuffer + LINE_BYTES * (targetY + y) + targetX / 2, imageData + imageLineBytes * y, imageLineBytes);
            // memset(getScreenLine(y + rect.y) + rect.x / 2, 0, rect.width / 2);
            // memset(getBackBufferLine(y + rect.y) + rect.x / 2, 0, rect.width / 2);
            memcpy(getBackBufferLine(y + rect.y) + rect.x, getBackgroundImageLine(y + rect.y) + rect.x, rect.width);
        }
    }
    m_dirtyRects.clear();
}

void VgaGfx::drawScreen()
{   
    vsync();

    // todo: Create minimal set of rectangles to redraw.

    for (int i = 0; i < m_undrawnRects.size(); ++i)
    {
        Rectangle& rect = m_undrawnRects[i];
        for (int y = 0; y < rect.height; ++y)
        {
            // memcpy(m_screenBuffer + LINE_BYTES * (targetY + y) + targetX / 2, imageData + imageLineBytes * y, imageLineBytes);
            memcpy(getScreenLine(y + rect.y) + rect.x, getBackBufferLine(y + rect.y) + rect.x, rect.width);
        }
    }

    m_undrawnRects.clear();
}


void VgaGfx::setBackground(const ImageBase& image)
{
    if (image.width() == SCREEN_W &&
        image.height() == SCREEN_H)
    {
        memcpy(m_backgroundImage, image.data(), SCREEN_W * SCREEN_H);
        // memcpy(m_screenBuffer, image.data(), image.width() * image.height() / 2);
        Rectangle rect(
            0, 0,
            image.width(), image.height());
        m_dirtyRects.push_back(rect);
    }
}




void VgaGfx::drawText(const char* text, int16_t targetX, int16_t targetY)
{
    int len = strlen(text);

    Rectangle rect(
        targetX, targetY,
        len * CHAR_WIDTH, CHAR_HEIGHT);
    // m_dirtyRects.push_back(rect);
    // m_undrawnRects.push_back(rect);


    for (int i = 0; i < len; ++i)
    {
        char* charImg = alphabet[getCharacterIndex(text[i])];
        for (int y = 0; y < CHAR_HEIGHT; ++y)
        {
            memcpy(getBackBufferLine(targetY + y) + targetX + i * CHAR_WIDTH, charImg + CHAR_WIDTH * y, CHAR_WIDTH);
        }
    }
}