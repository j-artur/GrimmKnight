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
const float KNOCKBACK_UP_SPEED = -128.0f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

const float LAYER_BG = 0.99f;
const float LAYER_LIGHT = 0.95f;
const float LAYER_FG = 0.90f;
const float LAYER_PLAYER = 0.85f;
const float LAYER_ENEMY = 0.80f;
const float LAYER_ENEMY_ATTACK = 0.75f;
const float LAYER_BOSS = 0.70f;
const float LAYER_BOSS_ATTACK = 0.65f;
const float LAYER_ATTACK = 0.60f;
const float LAYER_FIREBALL = 0.55f;
const float LAYER_HUD_BG = 0.50f;
const float LAYER_HUD_FG = 0.45f;
const float LAYER_TUTORIAL = 0.40f;
const float LAYER_TRANSITION_SCREEN = 0.20f;
const float LAYER_PAUSE_SCREEN = 0.10f;

#endif
