#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "rectangle.h"

#include <vector>
#include <stdint.h>

struct Actor
{
    int16_t x,y;
    int16_t width, height;
    int16_t dx, dy;
};

class Physics
{
public:

    int addActor(const Actor& rect);
    void setActor(int index, const Actor& rect);

    void addWall(const Rectangle& rect);

    void calc();

public:

    std::vector<Rectangle> m_walls;
    std::vector<Actor> m_actors;
};

#endif
