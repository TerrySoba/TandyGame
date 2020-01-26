#include "physics.h"

int Physics::addActor(const Actor &rect)
{
    m_actors.push_back(rect);
    return m_actors.size() - 1;
}

void Physics::setActor(int index, const Actor &rect)
{
    m_actors[index] = rect;
}

void Physics::setWalls(const tnd::vector<Rectangle>& walls)
{
    m_walls = walls;
}

void Physics::getActorPos(int index, int16_t& x, int16_t& y)
{
    Actor& actor = m_actors[index];
    x = actor.rect.x;
    y = actor.rect.y;
}

#define abs(x) (((x) > 0)?x:-x)

Rectangle extendRectangle(Rectangle rect, int16_t horizontal, int16_t vertical)
{
    rect.x -= horizontal;
    rect.width += horizontal * 2;
    rect.y -= vertical;
    rect.height += vertical * 2;
    return rect;
}

void Physics::calc()
{
    for (int i = 0; i < m_actors.size(); ++i)
    {
        Actor& actor = m_actors[i];


        Point oldPos(actor.rect.x, actor.rect.y);

        actor.rect.x += actor.dx;
        actor.rect.y += actor.dy;

        actor.dx *= 0.99;
        actor.dy *= 0.99;

        actor.isOnGround = false;

        for (int n = 0; n < m_walls.size(); ++n)
        {
            Rectangle& wall = m_walls[n];
            if (intersectRect(wall, actor.rect))
            {
                
                
                IntersectionType wallType = getIntersectionType(extendRectangle(wall, 0, actor.rect.height - 32), actor.rect);
                switch(wallType)
                {
                    case INTERSECTION_LEFT:
                        actor.dx = 0;
                        actor.rect.x = wall.x - actor.rect.width;
                        break;
                    case INTERSECTION_RIGHT:
                        actor.dx = 0;
                        actor.rect.x = wall.x + wall.width;
                        break;
                }

                
                IntersectionType groundType = getIntersectionType(extendRectangle(wall, actor.rect.width - 32, 0), actor.rect);
                
                switch(groundType)
                {
                    case INTERSECTION_TOP:
                        actor.rect.y = wall.y - actor.rect.height;
                        actor.dy = 0;
                        actor.isOnGround = true;
                        break;
                    case INTERSECTION_BOTTOM:
                        actor.rect.y = wall.y + wall.height;
                        actor.dy = abs(actor.dy);
                        break;

                }

            }
        }

        actor.dy += 3;
        if (actor.dy > 32) actor.dy = 32;
    }
}

void Physics::startActorJump(int index)
{
    Actor& actor = m_actors[index];
    // if (actor.jumpFrame == 0) actor.jumpFrame = 1;
    if (actor.isOnGround) actor.dy = -64;
}

void Physics::stopActorJump(int index)
{

}

void Physics::setActorSpeedX(int index, int16_t dx)
{
    Actor& actor = m_actors[index];
    actor.dx = dx;
}

void Physics::setActorSpeedY(int index, int16_t dy)
{
    Actor& actor = m_actors[index];
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
