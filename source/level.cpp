#include "level.h"
#include "csv_reader.h"
#include "blit.h"
#include "detect_lines.h"

Level::~Level()
{
}


Level::Level(const char* mapFilename, const char* groundFilename, shared_ptr<ImageBase> tilesImage,
             int16_t tileWidth, int16_t tileHeight,
             int16_t offsetX, int16_t offsetY) :
    m_tilesImage(tilesImage),
    m_tileWidth(tileWidth),
    m_tileHeight(tileHeight),
    m_offsetX(offsetX),
    m_offsetY(offsetY)
{
    {
        CsvReader<uint8_t> reader(mapFilename);
        m_mapWidth = reader.width();
        m_mapHeight = reader.height();
        m_mapData = reader.data();
    }
    {

        Point offset(m_offsetX, m_offsetY);

        CsvReader<uint8_t> bg(groundFilename);
        m_walls = detectLines(bg, HORIZONTAL, 1);
        for (int i = 0; i < m_walls.size(); ++i)
        {
            m_walls[i].scale(tileWidth * 16, tileHeight * 16);
            m_walls[i] += offset * 16;
        }

        m_death = detectLines(bg, HORIZONTAL, 3);
        for (int i = 0; i < m_death.size(); ++i)
        {
            m_death[i].scale(tileWidth * 16, tileHeight * 16);
            m_death[i] += offset * 16;
            m_death[i].shrink(20);
        }

        for (int x = 0; x < bg.width(); ++x)
        {
            for (int y = 0; y < bg.height(); ++y)
            {
                if (bg.get(x,y) == 2)
                {
                    m_spawn.x = x * tileWidth + m_offsetX;
                    m_spawn.y = y * tileHeight + m_offsetY;
                }
            }
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
    static const int16_t tileSetWidth = 20;
    static const int16_t tileSetHeight = 12;

    for (int i = 0; i < m_mapData.size(); ++i)
    {
        int index = m_mapData[i];
        Rectangle src((index % tileSetWidth) * m_tileWidth, (index / tileSetWidth) * m_tileHeight, m_tileWidth, m_tileHeight);
        Point pos((i % m_mapWidth) * m_tileWidth + x, (i / m_mapWidth) * m_tileHeight + y);
        blit(*m_tilesImage, src, target, pos);
    }
}
