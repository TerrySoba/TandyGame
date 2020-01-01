#ifndef _DRAWABLE_H_INCLUDED
#define _DRAWABLE_H_INCLUDED

#include "image_base.h"

class Drawable
{
public:
    virtual int16_t width() const = 0;
    virtual int16_t height() const = 0;
    virtual void draw(const ImageBase& target, int16_t x, int16_t y) const = 0;
};

#endif