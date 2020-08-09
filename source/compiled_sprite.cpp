#include "compiled_sprite.h"

#include "tga_image.h"
#include "log.h"

#include <string.h>

typedef void (*spriteFun)(char* img);

void write8Bit(char* function, int& functionPos, uint16_t offset, uint8_t data)
{
    strcpy(function + functionPos, "\xC6\x87"); // mov    byte ptr offset[bx],data
    functionPos += 2;

    memcpy(function + functionPos, &offset, 2);
    functionPos += 2;

    *(function + functionPos) = data;
    functionPos += 1;
}

void write16Bit(char* function, int& functionPos, uint16_t offset, uint16_t data)
{
    strcpy(function + functionPos, "\xC7\x87"); // mov    byte ptr offset[bx],data
    functionPos += 2;

    memcpy(function + functionPos, &offset, 2);
    functionPos += 2;

    memcpy(function + functionPos, &data, 2);
    functionPos += 2;
}


CompiledSprite::CompiledSprite(const char* filename, int16_t targetWidth)
{
    TgaImage img(filename);
    m_width = img.width();
    m_height = img.height();

    char* data = img.data();

    int functionSize = 0;
    for (int y = 0; y < img.height(); ++y)
    {
        int consecutivePixel = 0;
        for (int x = 0; x < img.width(); ++x)
        {
            if (data[y * img.width() + x] == 0)
            {
                if (consecutivePixel == 1)
                {
                    functionSize += 5;
                    consecutivePixel = 0;
                }
            }
            else
            {
                if (consecutivePixel == 0) consecutivePixel++;
                else
                {
                    functionSize += 6;
                    consecutivePixel = 0;
                }
                
            }
        }
        if (consecutivePixel != 0) functionSize += 5;
    }

    LOG_ENTRY("size: %d", functionSize);

    char* function = new char[functionSize + 10];
    int functionPos = 0;

    const char* functionHeader =
        "\x53"      // push bx
        "\x89\xc3"  // mov bx,ax
        "\x8E\xDA"; // mov ds,dx

    const char* functionEnd =
        "\x5b"      // pop bx
        "\xcb";     // retf

    strcpy(function, functionHeader);
    functionPos = strlen(function);


    char lastPixel;

    for (int y = 0; y < img.height(); ++y)
    {
        int consecutivePixel = 0;
        for (int x = 0; x < img.width(); ++x)
        {
            char pixel = data[y * img.width() + x];
            if (pixel == 0) // if pixel is transparent
            {
                if (consecutivePixel == 1)
                {
                    write8Bit(function, functionPos, targetWidth * y + x - 1, lastPixel);
                    consecutivePixel = 0;
                }
            }
            else
            {
                if (consecutivePixel == 0) consecutivePixel++;
                else
                {
                    write16Bit(function, functionPos, targetWidth * y + x - 1, lastPixel | pixel << 8);
                    consecutivePixel = 0;
                }
                
            }
            lastPixel = pixel;
        }
        if (consecutivePixel != 0)
        {
            write8Bit(function, functionPos, targetWidth * y + img.width() - 1, lastPixel);
        }
    }

    strcpy(function + functionPos, functionEnd);

    m_compiledFunction = function;
}


CompiledSprite::~CompiledSprite()
{
    delete m_compiledFunction;
}

int16_t CompiledSprite::width() const
{
    return m_width;
}

int16_t CompiledSprite::height() const
{
    return m_height;
}

/*
void sprite2(char* img)
{
    img[0 * 320 + 6] = 4;
    img[1 * 320 + 3] = 4;
}
*/

void CompiledSprite::draw(char* target, int16_t targetWidth, int16_t targetHeight, int16_t targetX, int16_t targetY) const
{
    char* img = target + targetWidth * targetY + targetX;
    ((spriteFun)m_compiledFunction)(img);
}
