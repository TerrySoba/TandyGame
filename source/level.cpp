#include "level.h"
#include "csv_reader.h"

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
