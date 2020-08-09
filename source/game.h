#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "shared_ptr.h"
#include "physics.h"
#include "physics_event.h"
#include "tiny_string.h"
#include "actor_animation_controller.h"

// forward declarations
class VgaGfx;
class ImageBase;
class Animation;
class CompiledSprite;

namespace UseSpawnPoint
{
	enum UseSpawnPointT
	{
		NO,
		YES
	};
}


class Game : public PhysicsCallback
{
public:
	Game(shared_ptr<VgaGfx> vgaGfx, shared_ptr<ImageBase> tiles, shared_ptr<Animation> actorAnimation, const char* levelBasename);

	void loadLevel(int levelNumber, UseSpawnPoint::UseSpawnPointT useSpawnPoint);
    void drawFrame();

	// PhysicsCallback interface
	virtual void levelTransition(LevelTransition transition);

private:
	shared_ptr<VgaGfx> m_vgaGfx;
	shared_ptr<ImageBase> m_tiles;
	shared_ptr<Animation> m_actorAnimation;
	shared_ptr<CompiledSprite> m_compiledSprite;
    shared_ptr<Physics> m_physics;
    long int m_frames;
    int m_player;
	TinyString m_levelBasename;
	int m_levelNumber;
	int m_nextLevel;
	ActorAnimationController m_animationController;
};


#endif