#pragma once

#include <vector>
#include <string>
#include <cstdint>

struct Frame
{
    uint16_t x,y;
    uint16_t width, height;
    uint16_t duration; // in milliseconds
};

struct FrameTag
{
    std::string name;
    uint16_t startFrame, endFrame;
};

struct AnimationInfo
{
    std::vector<Frame> frames;
    std::vector<FrameTag> tags;
};


AnimationInfo loadAnimation(const std::string& filename);
