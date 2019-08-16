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

void Physics::addWall(const Rectangle &rect)
{
    m_walls.push_back(rect);
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

        for (int n = 0; n < m_walls.size(); ++n)
        {
            if (intersectRect(m_walls[n], actor.rect))
            {
                actor.rect.x -= actor.dx;
                actor.rect.y -= actor.dy;
                actor.dy *= -1;
                actor.dx *= -1;
            }
        }

        actor.dy += 2;
    }
}

bool Physics::intersectRect(const Rectangle &r1, const Rectangle &r2)
{
    return !(r2.x > r1.x + r1.width ||
             r2.x + r2.width < r1.x ||
             r2.y > r1.y + r1.height ||
             r2.y + r2.height < r1.y);
}
