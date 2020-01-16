#include "physics.h"

// This variable contains the jump curve.
// The values are speed values for each frame.
const int16_t jumpCurve[] = 
{
    32, 31, 29, 28, 27, 26, 24, 23, 22, 20, 19, 18, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 3, 2, 1, 
    -2, -3, -4, -6, -7, -8, -10, -11, -12, -14, -15, -16, -17, -19, -20, -21, -23, -24, -25, -27, -28, -29, -30, -32
};

int16_t jumpCurveCount = 49;

int Physics::addActor(const Actor &rect)
{
    m_actors.push_back(rect);
    return m_actors.size() - 1;
}

void Physics::setActor(int index, const Actor &rect)
{
    m_actors[index] = rect;
}

void Physics::setWalls(const std::vector<Rectangle>& walls)
{
    m_walls = walls;
}

void Physics::getActorPos(int index, int16_t& x, int16_t& y)
{
    Actor& actor = m_actors[index];
    x = actor.rect.x;
    y = actor.rect.y;
}

void Physics::calc()
{
    for (int i = 0; i < m_actors.size(); ++i)
    {
        Actor& actor = m_actors[i];
        actor.rect.x += actor.dx;
        actor.rect.y += actor.dy;

        actor.dx *= 0.99;
        actor.dy *= 0.99;

        if (actor.jumpFrame > jumpCurveCount) actor.jumpFrame = jumpCurveCount;

        if (actor.jumpFrame > 0)
        {
            actor.rect.y -= jumpCurve[actor.jumpFrame - 1];
            ++actor.jumpFrame;
        }

        for (int n = 0; n < m_walls.size(); ++n)
        {
            if (intersectRect(m_walls[n], actor.rect))
            {
                IntersectionType type = getIntersectionType(m_walls[n], actor.rect);
                // actor.rect.x -= actor.dx;
                // actor.rect.y -= actor.dy;
                actor.jumpFrame = 0;
                actor.dy = 0;

                // switch(type)
                // {
                //     case INTERSECTION_TOP:
                //         // falthrough
                //     case INTERSECTION_BOTTOM:
                //         actor.dy = 0;
                //         actor.jumpFrame = 0;
                //         break;
                //     case INTERSECTION_LEFT:
                //         // fallthrough
                //     case INTERSECTION_RIGHT:
                //         actor.dx = 0;
                //     case INTERSECTION_OTHER:
                //         actor.dy = 0;
                //         actor.dx = 0;
                //         break;
                // }
                
            }
        }

        // actor.dy += 2;
    }
}

void Physics::startActorJump(int index)
{
    Actor& actor = m_actors[index];
    if (actor.jumpFrame == 0) actor.jumpFrame = 1;
}

void Physics::stopActorJump(int index)
{

}

void Physics::setActorSpeed(int index, int16_t dx, int16_t dy)
{
    Actor& actor = m_actors[index];
    actor.dx = dx;
    actor.dy = dy;
}

bool Physics::intersectRect(const Rectangle &r1, const Rectangle &r2)
{
    return !(r2.x > r1.x + r1.width ||
             r2.x + r2.width < r1.x ||
             r2.y > r1.y + r1.height ||
             r2.y + r2.height < r1.y);
}

bool pointIntersect(const Rectangle& r, int16_t x, int16_t y)
{
    return (x >= r.x && x <= r.x + r.width) &&
           (y >= r.y && y <= r.y + r.height);
}

IntersectionType Physics::getIntersectionType(const Rectangle &r1, const Rectangle &r2)
{
    bool topLeft = pointIntersect(r1, r2.x, r2.y);
    bool topRight = pointIntersect(r1, r2.x + r2.width, r2.y);
    bool bottomLeft = pointIntersect(r1, r2.x, r2.y + r2.height);
    bool bottomRight = pointIntersect(r1, r2.x + r2.width, r2.y + r2.height);

    if (bottomLeft && bottomRight && !topLeft && !topRight)
    {
        return INTERSECTION_TOP;
    }
    if (bottomLeft && !bottomRight && topLeft && !topRight)
    {
        return INTERSECTION_RIGHT;
    }
    if (!bottomLeft && bottomRight && !topLeft && topRight)
    {
        return INTERSECTION_LEFT;
    }
    if (!bottomLeft && !bottomRight && topLeft && topRight)
    {
        return INTERSECTION_BOTTOM;
    }


    return INTERSECTION_OTHER;
}
