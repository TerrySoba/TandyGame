#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "shared_ptr.h"

// forward declarations
class VgaGfx;
class ImageBase;
class Animation;
class Physics;

class Game
{
public:
	Game(shared_ptr<VgaGfx> vgaGfx, shared_ptr<ImageBase> tiles, shared_ptr<Animation> actorAnimation);

	void loadLevel(const char* levelBasename);
    void drawFrame();

private:
	shared_ptr<VgaGfx> m_vgaGfx;
	shared_ptr<ImageBase> m_tiles;
	shared_ptr<Animation> m_actorAnimation;
    shared_ptr<Physics> m_physics;
    long int m_frames;
    int m_player;
};


#endif