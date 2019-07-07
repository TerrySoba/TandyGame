#include <stdio.h>
#include <exception>
#include <sstream>
#include <string>
#include <iostream>

#include "animation_loader.h"

#include "nlohmann/json.hpp"

std::string loadFile(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp)
    {
        throw std::runtime_error("Could not open file " + filename);
    }

    uint8_t buf[1024];
    std::stringstream result;

    while (!feof(fp))
    {
        auto size = fread(buf, 1, 1024, fp);
        result << std::string((const char*)buf, size);
    }

    return result.str();
}

AnimationInfo loadAnimation(const std::string& filename)
{
    auto jsonData = loadFile(filename);
    auto json = nlohmann::json::parse(jsonData);

    // std::cout << json.dump() << std::endl;

    AnimationInfo anim;

    for (auto frame : json.at("frames"))
    {
        anim.frames.push_back(
            {
                frame.at("frame").at("x"),
                frame.at("frame").at("y"),
                frame.at("frame").at("w"),
                frame.at("frame").at("h"),
                frame.at("duration")
            }
        );
    }

    for (auto tag : json.at("meta").at("frameTags"))
    {
        anim.tags.push_back(
            {
                tag.at("name"),
                tag.at("from"),
                tag.at("to")
            }
        );
    }

    return anim;
}
