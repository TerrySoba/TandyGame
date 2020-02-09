#ifndef _PHYSICS_EVENT_INCLUDED_H
#define _PHYSICS_EVENT_INCLUDED_H

enum LevelTransition
{
    RIGHT = 1,
    LEFT  = 2
};

class PhysicsCallback
{
public:
    virtual void levelTransition(LevelTransition transition) = 0;
};

#endif