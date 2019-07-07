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
    
    fread(&m_width, 2, 1, fp);
    fread(&m_height, 2, 1, fp);

    m_data = new char[m_width * m_height / 2];

    fread(m_data, 1, m_width * m_height / 2, fp);

	fclose(fp);
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

char* Animation::data() const
{
    return m_data;
}

void Animation::nextFrame()
{

}

void Animation::useTag(uint16_t)
{

}
