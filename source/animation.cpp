#include "animation.h"
#include "json.h"
#include "exception.h"
#include "log.h"

#include <stdio.h>
#include <string.h>

Animation::Animation(const char* jsonFilename, const char* tgaFilename, bool transparent) :
    m_image(tgaFilename), m_transparent(transparent)
{
    Filename filename(jsonFilename);
    Json json(filename);
    JsonValue value = json.getRoot();
    if (!value.isObject())
    {
        throw Exception("Invalid json file: ", jsonFilename);
    }

    JsonValue frames = value.at("frames");

    for (int i = 0; i < frames.size(); ++i)
    {
        JsonValue frame = frames.at(i).at("frame");
        int x = frame.at("x").toInt();
        int y = frame.at("y").toInt();
        int w = frame.at("w").toInt();
        int h = frame.at("h").toInt();

        int duration = frames.at(i).at("duration").toInt();

        Frame f = {
            x, y, w, h, duration
        };

        m_frames.push_back(f);
    }

    JsonValue tags = value.at("meta").at("frameTags");
    for (int i = 0; i < tags.size(); ++i)
    {
        JsonValue tag = tags.at(i);
        FrameTag frameTag; //(, , tag.at("name").toString().c_str());
        frameTag.startFrame = tag.at("from").toInt();
        frameTag.endFrame = tag.at("to").toInt();
        frameTag.name = tag.at("name").toString(); //.c_str();
        m_tags.push_back(frameTag);
    }

    m_minFrame = 0;
    m_maxFrame = m_frames.size() - 1;
    m_currentFrame = m_maxFrame;
    nextFrame();
}

int16_t Animation::width() const
{
    return m_width;
}

int16_t Animation::height() const
{
    return m_height;
}

void Animation::nextFrame()
{
    m_currentFrame += 1;
    if (m_currentFrame > m_maxFrame) m_currentFrame = m_minFrame;
    m_width = m_frames[m_currentFrame].width;
    m_height = m_frames[m_currentFrame].height;
}

void Animation::useTag(int16_t tag)
{
    if (tag >= m_tags.size()) return;

    m_minFrame = m_tags[tag].startFrame;
    m_maxFrame = m_tags[tag].endFrame;
    m_currentFrame = m_minFrame;
}

void Animation::useTag(const char* name)
{
    for (int i = 0; i < m_tags.size(); ++i)
    {
        if (m_tags[i].name == name)
        {
            useTag(i);
            return;
        }
    }
}

tnd::vector<FrameTag> Animation::getTags()
{
    return m_tags;
}

void Animation::draw(char* target, int16_t targetWidth, int16_t targetHeight, int16_t targetX, int16_t targetY) const
{
    const Frame& frame = m_frames[m_currentFrame];
    char* data = m_image.data();

    int16_t imageWidth = m_image.width();
    int16_t frameHeight = frame.height;
    int16_t frameWidth = frame.width;
    int16_t frameY = frame.y;
    int16_t frameX = frame.x;

    if (!m_transparent)
    {
        for (int16_t y = 0; y < frameHeight; ++y)
        {
            memcpy(
                target + targetWidth * (targetY + y) + targetX,
                data + imageWidth * (frameY + y)  + frameX,
                frameWidth);
        }
    }
    else
    {
        for (int16_t y = 0; y < frameHeight; ++y)
        {
            char *dst = target + targetWidth * (targetY + y) + targetX;
            const char *src = data + imageWidth * (frameY + y) + frameX;

            for (int i = 0; i < frameWidth; ++i)
            {
                if (*src != 0)
                {
                    *dst++ = *src++;
                }
                else
                {
                    ++dst;
                    ++src;
                }
            }
        }
    }
}
