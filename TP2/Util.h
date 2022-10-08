#ifndef _UTIL_H_
#define _UTIL_H_

#include <cmath>

enum Sounds
{
    JETUP,
    LOOPUP,
    JETDOWN,
    LOOPDOWN,
    FIRE,
    EXPLOSION
};

enum Objects
{
    PLAYER,
    ATTACK,
    FIREBALL,
    CAMERA,
    WALL_TOP,
    WALL_BOTTOM,
    WALL_LEFT,
    WALL_RIGHT,
    ENEMY,
    ENEMY_ATTACK,
    ENTITY_BLOCK_TOP,
    ENTITY_BLOCK_BOTTOM,
    ENTITY_BLOCK_LEFT,
    ENTITY_BLOCK_RIGHT,
};

enum Direction
{
    LEFT = 1,
    RIGHT = 5,
};

enum AttackDirection
{
    ATK_UP = 1,
    ATK_DOWN = 3,
    ATK_LEFT = 9,
    ATK_RIGHT = 27,
};

const float KNOCKBACK_SPEED = 96.0f;
const float KNOCKBACK_UP_SPEED = 128.0f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

#endif
