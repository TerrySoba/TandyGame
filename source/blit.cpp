#include "blit.h"
#include <string.h>
#include <algorithm>

void blit(const ImageBase& source, const Rectangle& sourceRect, const ImageBase& target, const Point& targetPos)
{
    int16_t copyWidth = sourceRect.width;
    int16_t copyHeight = sourceRect.height;

    for (int16_t y = 0; y < copyHeight; ++y)
    {
        memcpy(
            target.linePtr(targetPos.y + y) + targetPos.x,
            source.linePtr(sourceRect.y + y) + sourceRect.x,
            copyWidth);
    }


}