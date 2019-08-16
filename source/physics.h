#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "rectangle.h"

#include <vector>
#include <stdint.h>

struct Actor
{
    Rectangle rect;
    int16_t dx, dy;
};

class Physics
{
public:

    int addActor(const Actor& rect);
    void setActor(int index, const Actor& rect);
    void getActorPos(int index, int16_t& x, int16_t& y);

    void addWall(const Rectangle& rect);

    void calc();

    static bool intersectRect(const Rectangle &r1, const Rectangle &r2);

public:

    std::vector<Rectangle> m_walls;
    std::vector<Actor> m_actors;
};

#endif
