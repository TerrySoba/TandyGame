#ifndef _COMPILED_SPRITE_H_INCLUDED
#define _COMPILED_SPRITE_H_INCLUDED

#include "drawable.h"


/**
 * A compiled sprite loads the given tga file and compiles it to
 * 8086 machine code. This makes drawing it faster, especially if
 * it contains transparency, bit it has an overhead of 2 to 4 bytes
 * per pixel in ram, depending on image content.
 * 
 * The target image width has to be known at construction time of the
 * sprite.
 */
class CompiledSprite : public Drawable
{
public:
    CompiledSprite(const char* filename, int16_t targetWidth);
    virtual ~CompiledSprite();

    virtual int16_t width() const;
    virtual int16_t height() const;
    virtual void draw(const ImageBase& target, int16_t x, int16_t y) const;

private:
    int16_t m_width, m_height;
    char* m_compiledFunction;
};

#endif