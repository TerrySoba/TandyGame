#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "shared_ptr.h"
#include "physics.h"
#include "physics_event.h"
#include "tiny_string.h"
#include "actor_animation_controller.h"
#include "enemy.h"

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

struct LevelNumber
{
	int8_t x;
	int8_t y;
};

class Game : public PhysicsCallback
{
public:
	Game(shared_ptr<VgaGfx> vgaGfx, shared_ptr<ImageBase> tiles, shared_ptr<Animation> actorAnimation, shared_ptr<Animation> enemyAnimation, const char* levelBasename);

	void loadLevel(LevelNumber levelNumber, UseSpawnPoint::UseSpawnPointT useSpawnPoint);
    void drawFrame();

	// PhysicsCallback interface
	virtual void levelTransition(LevelTransition transition);

private:
	shared_ptr<VgaGfx> m_vgaGfx;
	shared_ptr<ImageBase> m_tiles;
	shared_ptr<Animation> m_actorAnimation;
	shared_ptr<Animation> m_enemyAnimation;
	tnd::vector<shared_ptr<Enemy> > m_enemies;
    shared_ptr<Physics> m_physics;
    long int m_frames;
    int m_player;
	TinyString m_levelBasename;
	LevelNumber m_levelNumber;
	LevelNumber m_nextLevel;
	ActorAnimationController m_animationController;
};


#endif