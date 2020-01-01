#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include "image_base.h"
#include "drawable.h"
#include "shared_ptr.h"

#include <vector>

class Level : public Drawable
{
public:
    Level(const char* mapFilename, shared_ptr<ImageBase> tilesImage, int tileWidth, int tileHeight);

    virtual int16_t width() const;
    virtual int16_t height() const;
    virtual void draw(const ImageBase& target, int16_t x, int16_t y) const;

private:
    shared_ptr<ImageBase> m_tilesImage;
    int m_tileWidth;  // width of a single tile in pixles
    int m_tileHeight; // height of a single tile in pixles

    int m_mapWidth;   // number of tiles in horizontal direction
    int m_mapHeight;  // number of tiles in vertical direction
    std::vector<uint8_t> m_mapData;
};

#endif
