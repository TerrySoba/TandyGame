#include "game.h"
#include "keyboard.h"
#include "level.h"
#include "image_base.h"
#include "animation.h"
#include "vgagfx.h"
#include "physics.h"
#include "version.h"
#include "vector.h"
#include "exception.h"
#include "compiled_sprite.h"

#include <stdio.h>

Game::Game(shared_ptr<VgaGfx> vgaGfx, shared_ptr<ImageBase> tiles,
           shared_ptr<Animation> actorAnimation, const char* levelBasename) :
    m_vgaGfx(vgaGfx), m_tiles(tiles), m_actorAnimation(actorAnimation),
    m_frames(0), m_levelBasename(levelBasename), m_nextLevel(-1),
    m_animationController(actorAnimation)
{
    m_compiledSprite = shared_ptr<CompiledSprite>(new CompiledSprite("guyframe.tga", 320));
}

void Game::loadLevel(int levelNumber, UseSpawnPoint::UseSpawnPointT useSpawnPoint)
{
    m_levelNumber = levelNumber;
    tnd::vector<char> buf(100);

    sprintf(buf.data(), m_levelBasename.c_str(), m_levelNumber);

    TinyString levelBg = TinyString(buf.data() + TinyString("_bg.csv"));
    TinyString levelCol = TinyString(buf.data() + TinyString("_col.csv"));

    Level level(levelBg.c_str(), levelCol.c_str(), m_tiles, 16, 16, -8, -8);
    m_actorAnimation->useTag("LoopR");

    m_vgaGfx->drawBackground(level, -8, -8);

    int16_t actorPosX, actorPosY;
    if (m_physics.get() == 0 || useSpawnPoint == UseSpawnPoint::YES)
    {
        // so no previous position existed, or use of spawn point was explicitele requested
        // because of that we just put the actor to the define spawn point of the level
        actorPosX = PIXEL_TO_SUBPIXEL(level.getSpawnPoint().x);
        actorPosY = PIXEL_TO_SUBPIXEL(level.getSpawnPoint().y);
    }
    else
    {
        // we use the y position of the actor from the previous level
        m_physics->getActorPos(m_player, actorPosX, actorPosY);
        if (SUBPIXEL_TO_PIXEL(actorPosX) > 160)
        {
            actorPosX = PIXEL_TO_SUBPIXEL(10);
        }
        else
        {
            actorPosX = PIXEL_TO_SUBPIXEL(310 - m_actorAnimation->width());
        }
    }
    
    m_physics.reset(); // reset first, so we do not have two instances of physics at once
    m_physics = shared_ptr<Physics>(new Physics(this));
    Actor actor;
    actor.rect.x = actorPosX;
    actor.rect.y = actorPosY;
    actor.rect.width = PIXEL_TO_SUBPIXEL(m_actorAnimation->width());
    actor.rect.height = PIXEL_TO_SUBPIXEL(m_actorAnimation->height());
    actor.dx = 0;
    actor.dy = 0;
    actor.jumpFrame = 1;
    m_player = m_physics->addActor(actor);

    m_physics->setWalls(level.getWalls());
    m_physics->setDeath(level.getDeath());
    m_physics->setSpawnPoint(level.getSpawnPoint() * 16);

    m_vgaGfx->clear();

    
    snprintf(buf.data(), buf.size(), "Build date: %s", BUILD_DATE);
    m_vgaGfx->drawText(buf.data(), 50, 193);
}


void Game::drawFrame()
{
    if (m_nextLevel >= 0)
    {
        loadLevel(m_nextLevel, UseSpawnPoint::NO);
        m_nextLevel = -1;
    }

    m_vgaGfx->clear();

    int16_t playerX;
    int16_t playerY;
    m_physics->getActorPos(m_player, playerX, playerY);

    m_animationController.setPos(playerX, playerY);

    // m_vgaGfx->draw(*m_compiledSprite, SUBPIXEL_TO_PIXEL(playerX), SUBPIXEL_TO_PIXEL(playerY));
    m_vgaGfx->draw(*m_compiledSprite, SUBPIXEL_TO_PIXEL(playerX) + 10, SUBPIXEL_TO_PIXEL(playerY));
    // m_vgaGfx->draw(*m_compiledSprite, SUBPIXEL_TO_PIXEL(playerX) + 20, SUBPIXEL_TO_PIXEL(playerY));
    // m_vgaGfx->draw(*m_compiledSprite, SUBPIXEL_TO_PIXEL(playerX) + 30, SUBPIXEL_TO_PIXEL(playerY));

    // m_vgaGfx->draw(*m_actorAnimation, 10, 10);
    // m_vgaGfx->draw(*m_actorAnimation, 30, 10);
    // m_vgaGfx->draw(*m_actorAnimation, 50, 10);
    // m_vgaGfx->draw(*m_actorAnimation, 70, 10);

    m_vgaGfx->draw(*m_actorAnimation, SUBPIXEL_TO_PIXEL(playerX), SUBPIXEL_TO_PIXEL(playerY));
    // m_vgaGfx->draw(*m_actorAnimation, SUBPIXEL_TO_PIXEL(playerX) + 10, SUBPIXEL_TO_PIXEL(playerY));
    m_vgaGfx->drawScreen();

    ++m_frames;

    if (s_keyRight)
    {
        m_physics->setActorSpeedX(m_player, 16);
    }
    
    if (s_keyLeft)
    {
        m_physics->setActorSpeedX(m_player, -16);
    }

    if (s_keyAlt)
    {
        m_physics->startActorJump(m_player);
    }

    if (m_frames % 4 == 0) m_actorAnimation->nextFrame();
    m_physics->calc();
}

void Game::levelTransition(LevelTransition transition)
{
    switch(transition)
    {
        case RIGHT:
            m_nextLevel = m_levelNumber + 1;
            break;
        case LEFT:
            m_nextLevel = m_levelNumber - 1;
            break;
    }
}
