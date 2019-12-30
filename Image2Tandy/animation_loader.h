#pragma once

#include <vector>
#include <string>
#include <cstdint>

struct Frame
{
    int16_t x,y;
    int16_t width, height;
    int16_t duration; // in milliseconds
};

struct FrameTag
{
    std::string name;
    int16_t startFrame, endFrame;
};

struct AnimationInfo
{
    std::vector<Frame> frames;
    std::vector<FrameTag> tags;
};


AnimationInfo loadAnimation(const std::string& filename);
