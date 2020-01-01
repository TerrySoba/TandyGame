#include "level.h"
#include "csv_reader.h"
#include "blit.h"


Level::Level(const char* mapFilename, shared_ptr<ImageBase> tilesImage, int tileWidth, int tileHeight) :
    m_tilesImage(tilesImage),
    m_tileWidth(tileWidth),
    m_tileHeight(tileHeight)
{
    CsvReader reader(mapFilename);
    m_mapWidth = reader.width();
    m_mapHeight = reader.height();

    m_mapData.resize(m_mapWidth * m_mapHeight);

    for (int y = 0; y < m_mapHeight; ++y)
    {
        for (int x = 0; x < m_mapWidth; ++x)
        {
            m_mapData[x + y*m_mapWidth] = reader.get(x, y);
        }
    }
}

int16_t Level::width() const
{
    return m_tileWidth * m_mapWidth;
}

int16_t Level::height() const
{
    return m_tileHeight * m_mapHeight;
}


void Level::draw(const ImageBase& target, int16_t x, int16_t y) const
{
    for (int i = 0; i < m_mapData.size(); ++i)
    {
        int index = m_mapData[i];
        Rectangle src((index % m_mapWidth) * m_tileWidth, (index / m_mapWidth) * m_tileHeight, m_tileWidth, m_tileHeight);
        Point pos((i % m_mapWidth) * m_tileWidth, (i / m_mapWidth) * m_tileHeight);
        blit(*m_tilesImage, src, target, pos);
    }
}
