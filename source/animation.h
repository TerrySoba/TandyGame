#ifndef _ANIMATION_H_INC_
#define _ANIMATION_H_INC_

#include "tga_image.h"
#include "image.h"

#include <stdint.h>
#include <vector>


struct Frame
{
    int16_t x,y;
    int16_t width, height;
    int16_t duration; // in milliseconds
};

struct FrameTag
{
    int16_t startFrame, endFrame;
    std::string name;
};

class Animation
{
public:
    Animation(const char* jsonFilename, const char* tgaFilename);

    int16_t width() const;
	int16_t height() const;

    void nextFrame();
    void useTag(int16_t);
    void useTag(const char* name);

    void draw(char* dest, int16_t lineLength, int16_t x, int16_t y) const;
    void drawTransparent(char* dest, int16_t lineLength, int16_t x, int16_t y) const;

private:
    int16_t m_width;
    int16_t m_height;

    std::vector<Frame> m_frames;
    std::vector<FrameTag> m_tags;
    int m_currentFrame;
    int m_minFrame;
    int m_maxFrame;

    TgaImage m_image;
};

#endif
