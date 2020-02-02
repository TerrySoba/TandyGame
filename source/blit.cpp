#include "blit.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "exception.h"
// #include <algorithm>
#include <sstream>

void blit(const ImageBase& source, const Rectangle& sourceRect, const ImageBase& target, const Point& targetPos)
{
    Rectangle src = Rectangle(0,0,source.width(), source.height()).intersection(sourceRect);
    Rectangle dest = Rectangle(0,0,target.width(), target.height())
                     .intersection(Rectangle(src - Point(sourceRect.x, sourceRect.y) + targetPos));
    src = src.intersection(dest - targetPos + Point(sourceRect.x, sourceRect.y));

    int16_t copyWidth = dest.width;
    int16_t copyHeight = dest.height;

    for (int16_t y = 0; y < copyHeight; ++y)
    {
        memcpy(
            target.linePtr(dest.y + y) + dest.x,
            source.linePtr(src.y + y) + src.x,
            copyWidth);
    }
}
