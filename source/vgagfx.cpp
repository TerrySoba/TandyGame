#include "vgagfx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>

#define inport(px) inpw(px)
#define inportb(px) inp(px)

#define SCREEN_W 320L
#define SCREEN_H 200L


extern void videoInit(void);
#pragma aux videoInit =    \
    "mov ah, 00h"          \
    "mov al, 13h"          \
    "int 10h"              \
    ;


// static void videoInit()
// {
//     __asm{
//         mov ah, 00h
//         mov al, 09h
//         int 10h
//        }
// }


// uint16_t segment(const char far* p)
// {
//     return (((int32_t)p) & 0xffff0000) >> 16;
// }

// uint16_t ptr(const char far* p)
// {
//     return (((int32_t)p) & 0x0000ffff);
// }

// // extern void myMemCopy(char far* dest, const char far* src, int16_t len);
// extern void myMemCopy(char far* dest, const char far* src, uint16_t len)
// {
//     uint16_t destSeg = segment(dest);
//     uint16_t destPtr = ptr(dest);
//     uint16_t srcSeg = segment(src);
//     uint16_t srcPtr = ptr(src);


//     __asm{
//         mov ax, len
//         mov bx, destPtr
//         mov ds, destSeg
//         mov cx, 00102h
//         copyloop:
//         mov [bx], ch
//         inc bx
//         dec ax
//         jnz copyloop
//     }

//     // uint16_t color = 0x0102;
//     // __asm{
//     //     mov ax, 0A000h
//     //     mov ds, ax
//     //     mov cx, color
//     //     mov bx, 100
//     //     mov [bx], cx
//     // }
// }

// #pragma aux myMemCopy =    \
//     "MOV AX, 0A000h" \
//     "MOV DS, AX" \
//     ";MOV CX, 00304h" \
//     "MOV BX, 100 " \
//     "MOV [BX], CX " \
//     parm   [cx]  \
//     ;



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

    Rectangle rect = {0, 0, SCREEN_W, SCREEN_H};
    m_dirtyRects.push_back(rect);

    videoInit();
}

VgaGfx::~VgaGfx()
{
    delete[] m_screenBuffer;
    delete[] m_backgroundImage;
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


void VgaGfx::drawImage(const ImageBase& image, int targetX, int targetY)
{
    const char* imageData = image.data();
    const int imageLineBytes = image.width();

    Rectangle rect = {
        targetX, targetY,
        image.width(), image.height()};
    m_dirtyRects.push_back(rect);
    m_undrawnRects.push_back(rect);

    for (int y = 0; y < image.height(); ++y)
    {
        memcpy(getBackBufferLine(y + targetY) + targetX, imageData + imageLineBytes * y, imageLineBytes);
    }
}

void VgaGfx::drawImageTransparent(const ImageBase& image, int targetX, int targetY, uint8_t transparentColor)
{
    const char* imageData = image.data();
    const int imageLineBytes = image.width();

    Rectangle rect = {
        targetX, targetY,
        image.width(), image.height()};
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

        // memcpy(getBackBufferLine(y + targetY) + targetX, imageData + imageLineBytes * y, imageLineBytes);
        
        // myMemCopy(getBackBufferLine(y + targetY) + targetX, imageData + imageLineBytes * y, imageLineBytes);
        // myMemCopy(0x0202);

        // for (int x = 0; x < image.width(); ++x)
        // {
        //     uint8_t pixel = *(imageData + imageLineBytes * y + x);
        //     if (pixel != transparentColor)
        //         line[targetX + x] = pixel;
        // }
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
        Rectangle rect = {
            0, 0,
            image.width(), image.height()};
        m_dirtyRects.push_back(rect);
    }
}
