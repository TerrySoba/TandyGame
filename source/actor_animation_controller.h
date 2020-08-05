#ifndef ACTOR_ANIMATION_CONTROLLER_H_INCLUDED
#define ACTOR_ANIMATION_CONTROLLER_H_INCLUDED

#include "animation.h"
#include "shared_ptr.h"

enum AnimationEnum
{
    ANIM_INITIAL,
    ANIM_WALK_RIGHT,
    ANIM_WALK_LEFT,
    ANIM_STAND,
    ANIM_JUMP_RIGHT,
    ANIM_JUMP_LEFT,
};

enum Direction
{
    UP,
    DOWN
};

#define my_abs(x) ((x > 0)?(x):-(x))


class ActorAnimationController
{
public:
    ActorAnimationController(shared_ptr<Animation> actorAnimation) :
        m_actorAnimation(actorAnimation), m_lastX(0), m_lastY(0), m_activeAnimation(ANIM_INITIAL), m_lastDirection(DOWN), airFrames(0)
    {
    }
    
    void setPos(int16_t x, int16_t y)
    {
        int16_t dx = x - m_lastX;
        int16_t dy = y - m_lastY;
        m_lastX = x;
        m_lastY = y;

        Direction direction = (dy > 0) ? DOWN : UP;

        AnimationEnum nextAnim = m_activeAnimation;

        if (my_abs(dy) < 2)
        {
            if (m_lastDirection == DOWN || airFrames++ > 4)
            {
                airFrames = 0;
                if (dx > 0)
                {
                    nextAnim = ANIM_WALK_RIGHT;
                }
                else if (dx < 0)
                {
                    nextAnim = ANIM_WALK_LEFT;
                }
                else if (dx == 0)
                {
                    nextAnim = ANIM_STAND;
                }
            }
            else
            {
                ++airFrames;
                nextAnim = m_activeAnimation;
            }
        }
        else
        {
            if (dx >= 0)
            {
                nextAnim = ANIM_JUMP_RIGHT;
            }
            else
            {
                nextAnim = ANIM_JUMP_LEFT;
            }
        }
        
        if (dy != 0) m_lastDirection = direction;

        if (nextAnim != m_activeAnimation)
        {
            m_activeAnimation = nextAnim;
            switch (nextAnim)
            {
            case ANIM_WALK_RIGHT:
                m_actorAnimation->useTag("WalkR");
                break;
            case ANIM_WALK_LEFT:
                m_actorAnimation->useTag("WalkL");
                break;
            case ANIM_STAND:
                m_actorAnimation->useTag("Stand");
                break;
            case ANIM_JUMP_RIGHT:
                m_actorAnimation->useTag("JumpR");
                break;
            case ANIM_JUMP_LEFT:
                m_actorAnimation->useTag("JumpL");
                break;
            }
        }

    }

private:
    shared_ptr<Animation> m_actorAnimation;
    int16_t m_lastX;
    int16_t m_lastY;
    AnimationEnum m_activeAnimation;
    Direction m_lastDirection;
    int airFrames;
};

#endif