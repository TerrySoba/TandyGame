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
#include "joystick.h"

#include <stdio.h>

Game::Game(shared_ptr<VgaGfx> vgaGfx, shared_ptr<ImageBase> tiles,
           GameAnimations animations,
           const char* levelBasename) :
    m_vgaGfx(vgaGfx), m_tiles(tiles), m_animations(animations), m_frames(0), m_levelBasename(levelBasename),
    m_animationController(animations.actorAnimation), m_lastButtonPressed(false)
{
    m_nextLevel.x = -1;
    m_nextLevel.y = -1;
}

void Game::loadLevel(LevelNumber levelNumber, UseSpawnPoint::UseSpawnPointT useSpawnPoint)
{
    LevelNumber previousLevel = m_levelNumber;
    m_levelNumber = levelNumber;
    tnd::vector<char> buf(100);

    sprintf(buf.data(), m_levelBasename.c_str(), m_levelNumber.x, m_levelNumber.y);

    TinyString levelBg = TinyString(buf.data() + TinyString("_bg.csv"));
    TinyString levelCol = TinyString(buf.data() + TinyString("_col.csv"));

    Level level(levelBg.c_str(), levelCol.c_str(), m_tiles, 16, 16, -8, -8);
    m_animations.actorAnimation->useTag("LoopR");

    m_vgaGfx->drawBackground(level, -8, -8);

    int16_t actorPosX, actorPosY;
    if (m_physics.get() == 0 || useSpawnPoint == UseSpawnPoint::YES)
    {
        // so no previous position existed, or use of spawn point was explicitly requested
        // because of that we just put the actor to the defined spawn point of the level
        actorPosX = PIXEL_TO_SUBPIXEL(level.getSpawnPoint().x);
        actorPosY = PIXEL_TO_SUBPIXEL(level.getSpawnPoint().y);
    }
    else
    {
        // we use the y position of the actor from the previous level
        m_physics->getActorPos(m_player, actorPosX, actorPosY);
        if (previousLevel.x < levelNumber.x) // left to right
        {
            actorPosX = PIXEL_TO_SUBPIXEL(10);
        }
        else if (previousLevel.x > levelNumber.x) // right to left
        {
            actorPosX = PIXEL_TO_SUBPIXEL(310 - m_animations.actorAnimation->width());
        }
        else if (previousLevel.y < levelNumber.y) // top to bottom
        {
            actorPosY = PIXEL_TO_SUBPIXEL(10);
        }
        else if (previousLevel.y > levelNumber.y) // bottom to top
        {
            actorPosY = PIXEL_TO_SUBPIXEL(185 - m_animations.actorAnimation->height());
        }
    }
    
    // now load enemies
    m_enemies.clear();
    tnd::vector<Rectangle> enemyRectangles = level.getEnemies();
    for (int i = 0; i < enemyRectangles.size(); ++i)
    {
        m_enemies.push_back(shared_ptr<Enemy>(new Enemy(enemyRectangles[i], m_animations.enemyAnimation)));
    }

    m_guffins.clear();
    m_guffins = level.getMacGuffins();

    m_physics.reset(); // reset first, so we do not have two instances of physics at once
    m_physics = shared_ptr<Physics>(new Physics(this));
    Actor actor;
    actor.rect.x = actorPosX;
    actor.rect.y = actorPosY;
    actor.rect.width = PIXEL_TO_SUBPIXEL(m_animations.actorAnimation->width());
    actor.rect.height = PIXEL_TO_SUBPIXEL(m_animations.actorAnimation->height());
    actor.dx = 0;
    actor.dy = 0;
    actor.jumpFrame = 1;
    m_player = m_physics->addActor(actor);

    m_physics->setWalls(level.getWalls());
    m_physics->setDeath(level.getDeath());
    m_physics->setFallThrough(level.getFallThrough());
    
    m_physics->setSpawnPoint(Point(actorPosX, actorPosY));

    m_vgaGfx->clear();

    
    snprintf(buf.data(), buf.size(), "Build date: %s", BUILD_DATE);
    m_vgaGfx->drawText(buf.data(), 50, 193);
    drawAppleCount();
}

void Game::drawAppleCount()
{
    char buf[16];
    snprintf(buf, 16, "Apples:%02d", m_collectedGuffins.size());
    m_vgaGfx->drawText(buf, 270, 1);
}


void Game::drawFrame()
{
    if (m_nextLevel.x != -1)
    {
        loadLevel(m_nextLevel, UseSpawnPoint::NO);
        m_nextLevel.x = -1;
    }

    m_vgaGfx->clear();

    int16_t playerX;
    int16_t playerY;
    m_physics->getActorPos(m_player, playerX, playerY);

    m_animationController.setPos(playerX, playerY);

    
    
    tnd::vector<Rectangle> enemyDeath;

    for (int i = 0; i < m_enemies.size(); ++i)
    {
        m_enemies[i]->walk();
        Rectangle enemy = m_enemies[i]->getPos();
        enemyDeath.push_back(enemy);
        m_vgaGfx->draw(*m_animations.enemyAnimation, SUBPIXEL_TO_PIXEL(enemy.x), SUBPIXEL_TO_PIXEL(enemy.y));
    }

    for (int i = 0; i < m_guffins.size(); ++i)
    {
        Rectangle& guffin = m_guffins[i];
        m_vgaGfx->draw(*m_animations.guffinAnimation, SUBPIXEL_TO_PIXEL(guffin.x), SUBPIXEL_TO_PIXEL(guffin.y));
    }

    m_vgaGfx->draw(*m_animations.actorAnimation, SUBPIXEL_TO_PIXEL(playerX), SUBPIXEL_TO_PIXEL(playerY));

    m_vgaGfx->drawScreen();
    
    m_physics->setEnemyDeath(enemyDeath);

    ++m_frames;

    uint8_t joystick = readJoystick();


    if (s_keyRight || joystick & JOY_RIGHT)
    {
        m_physics->setActorSpeedX(m_player, 16);
    }
    
    if (s_keyLeft || joystick & JOY_LEFT)
    {
        m_physics->setActorSpeedX(m_player, -16);
    }

    bool buttonPressed = s_keyAlt || joystick & JOY_BUTTON_1;
    if (buttonPressed && !m_lastButtonPressed)
    {
        m_physics->startActorJump(m_player);
    }
    m_lastButtonPressed = buttonPressed;


    if (s_keyDown || joystick & JOY_DOWN)
    {
        m_physics->setActorDuck(m_player, true);
    }
    else
    {
        m_physics->setActorDuck(m_player, false);
    }
    

    if (m_frames % 4 == 0)
    {
        m_animations.actorAnimation->nextFrame();
        m_animations.enemyAnimation->nextFrame();
        
    }

    if (m_frames % 16 == 0) m_animations.guffinAnimation->nextFrame();

    m_physics->calc();
}

void Game::levelTransition(LevelTransition transition)
{
    switch(transition)
    {
        case RIGHT:
            m_nextLevel = m_levelNumber;
            m_nextLevel.x += 1;
            break;
        case LEFT:
            m_nextLevel = m_levelNumber;
            m_nextLevel.x -= 1;
            break;
        case BOTTOM:
            m_nextLevel = m_levelNumber;
            m_nextLevel.y += 1;
            break;
        case TOP:
            m_nextLevel = m_levelNumber;
            m_nextLevel.y -= 1;
            break;
    }
}
