#ifndef _ANIMATION_H_INC_
#define _ANIMATION_H_INC_

#include "image.h"

#include <stdint.h>
#include <vector>


struct Frame
{
    uint16_t x,y;
    uint16_t width, height;
    uint16_t duration; // in milliseconds
};

struct FrameTag
{
    uint16_t startFrame, endFrame;
};

class Animation : public ImageBase
{
public:
    Animation(const char* filename);
    ~Animation();

    uint16_t width() const;
	uint16_t height() const;
	char* data() const;

    void nextFrame();
    void useTag(uint16_t);
private:
    uint16_t m_width;
    uint16_t m_height;
    char* m_data;

    std::vector<Frame> m_frames;
    std::vector<FrameTag> m_tags;
};

#endif
