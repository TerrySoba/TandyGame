#include "animation.h"

#include <stdio.h>
#include <string.h>
#include <stdexcept>

Animation::Animation(const char* filename)
{
    FILE* fp = fopen(filename, "rb");

	if (!fp)
	{
        throw std::runtime_error("Could not open file.");
	}

    char header[4];
    fread(header, 4, 1, fp);

    // check file header
    if (memcmp(header, "TANI", 4) != 0)
    {
        throw std::runtime_error("Animation file is missing correct header.");
    }

    uint16_t frameCount;
    fread(&frameCount, 2, 1, fp);

    for (int i = 0; i < frameCount; ++i)
    {
        Frame frame;
        fread(&frame.x, 2, 1, fp);
        fread(&frame.y, 2, 1, fp);
        fread(&frame.width, 2, 1, fp);
        fread(&frame.height, 2, 1, fp);
        fread(&frame.duration, 2, 1, fp);
        m_frames.push_back(frame);
    }

    uint16_t tagCount;
    fread(&tagCount, 2, 1, fp);

    for (int i = 0; i < tagCount; ++i)
    {
        FrameTag tag;
        fread(&tag.startFrame, 2, 1, fp);
        fread(&tag.endFrame, 2, 1, fp);
        m_tags.push_back(tag);
    }
    
    fread(&m_imageWidth, 2, 1, fp);
    fread(&m_imageHeight, 2, 1, fp);

    m_data = new char[m_imageWidth * m_imageHeight];

    fread(m_data, 1, m_imageWidth * m_imageHeight, fp);

	fclose(fp);

    
    m_minFrame = 0;
    m_maxFrame = frameCount - 1;
    m_currentFrame = m_maxFrame;
    nextFrame();
}


Animation::~Animation()
{
    delete m_data;
}

uint16_t Animation::width() const
{
    return m_width;
}

uint16_t Animation::height() const
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

void Animation::useTag(uint16_t tag)
{
    if (tag >= m_tags.size()) return;

    m_minFrame = m_tags[tag].startFrame;
    m_maxFrame = m_tags[tag].endFrame;
    m_currentFrame = m_minFrame;
}

void Animation::draw(char* dest, uint16_t lineLength, uint16_t targetX, uint16_t targetY) const
{
    const Frame& frame = m_frames[m_currentFrame];
    char* data = m_data;
    for (int y = 0; y < frame.height; ++y)
    {
        memcpy(
            dest + lineLength * (targetY + y) + targetX,
            data + m_imageWidth * (frame.y + y)  + frame.x,
            frame.width);
    }
}
