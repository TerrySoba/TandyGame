#include "level.h"
#include "csv_reader.h"
#include "blit.h"
#include "detect_lines.h"


Level::Level(const char* mapFilename, const char* groundFilename, shared_ptr<ImageBase> tilesImage, int tileWidth, int tileHeight) :
    m_tilesImage(tilesImage),
    m_tileWidth(tileWidth),
    m_tileHeight(tileHeight)
{
    {
        CsvReader<uint8_t> reader(mapFilename);
        m_mapWidth = reader.width();
        m_mapHeight = reader.height();
        m_mapData = reader.data();
    }
    {
        CsvReader<uint8_t> bg(groundFilename);
        m_walls = detectLines(bg, HORIZONTAL, 1);
        for (int i = 0; i < m_walls.size(); ++i)
            m_walls[i].scale(tileWidth * 16, tileHeight * 16);

        m_death = detectLines(bg, HORIZONTAL, 3);
        for (int i = 0; i < m_death.size(); ++i)
            m_death[i].scale(tileWidth * 16, tileHeight * 16);

        for (int x = 0; x < bg.width(); ++x)
        {
            for (int y = 0; y < bg.height(); ++y)
            {
                if (bg.get(x,y) == 2)
                {
                    m_spawn.x = x * tileWidth;
                    m_spawn.y = y * tileHeight;
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
    for (int i = 0; i < m_mapData.size(); ++i)
    {
        int index = m_mapData[i];
        Rectangle src((index % m_mapWidth) * m_tileWidth, (index / m_mapWidth) * m_tileHeight, m_tileWidth, m_tileHeight);
        Point pos((i % m_mapWidth) * m_tileWidth, (i / m_mapWidth) * m_tileHeight);
        blit(*m_tilesImage, src, target, pos);
    }
}
