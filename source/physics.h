#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "rectangle.h"

#include <vector>
#include <stdint.h>

// Physics calculations are done on subpixel.
// One pixel constitutes 16 subpixels, so shifting by 4
// is used to transform between pixels and subpixels.

#define PIXEL_TO_SUBPIXEL(x) ((x) << 4)
#define SUBPIXEL_TO_PIXEL(x) ((x) >> 4)

struct Actor
{
    Actor() :
        jumpFrame(0),
        dx(0), dy(0)
    {
    }


    Rectangle rect;
    int16_t dx, dy;
    int16_t jumpFrame;
};

enum IntersectionType
{
    INTERSECTION_TOP = 1,
    INTERSECTION_BOTTOM = 2,
    INTERSECTION_LEFT = 3,
    INTERSECTION_RIGHT = 4,
    INTERSECTION_OTHER = 5
};


class Physics
{
public:

    int addActor(const Actor& rect);
    void setActor(int index, const Actor& rect);
    void getActorPos(int index, int16_t& x, int16_t& y);
    void setActorSpeed(int index, int16_t dx, int16_t dy);
    void startActorJump(int index);
    void stopActorJump(int index);

    void addWall(const Rectangle& rect);

    void calc();

    static bool intersectRect(const Rectangle &r1, const Rectangle &r2);

    /**
     * Get type of intersection between r1 and r2.
     * r1 is the reference object so if the right side of r2 intersects
     * with the left side of r1 you get INTERSECTION_LEFT.
     */
    static IntersectionType getIntersectionType(const Rectangle &r1, const Rectangle &r2);

public:

    std::vector<Rectangle> m_walls;
    std::vector<Actor> m_actors;
};

#endif
