#ifndef _UTIL_H_
#define _UTIL_H_

#include <cmath>

#define M_PI 3.141592653589793238462643383279502884L

enum Sounds
{
    JETUP,
    LOOPUP,
    JETDOWN,
    LOOPDOWN,
    FIRE,
    EXPLOSION,

    PLAYER_ATTACK,
    PLAYER_SPIKE_ATTACK,
    PLAYER_JUMP,
    PLAYER_LAND,
    PLAYER_DASH,
    PLAYER_FIREBALL,
    PLAYER_HURT,
    PLAYER_WALK,

    ENEMY_DAMAGE,
    ENEMY_DREAM_DAMAGE,
    ENEMY_CRAWLER_1,
    ENEMY_CRAWLER_2,
    ENEMY_CRAWLER_3,
    ENEMY_FOOTSTEP_1,
    ENEMY_FOOTSTEP_2,
    ENEMY_RUN,
    ENEMY_DEATH,

    MAIN_MUSIC,
    ABYSS_MUSIC,
    LEVEL3_MUSIC,
    OPENING,
    CAVE_NOISES,
    CAVE_WIND,

    SFK_THEME,
    SFK_STUN,
    SFK_ARMOR_DAMAGE,
    SFK_HEAD_DAMAGE,
    SFK_JUMP,
    SFK_LAND,
    SFK_SWING,
    SFK_STRIKE_GROUND,
    SFK_VOICE_ATTACK,
    SFK_VOICE_RAGE,
    SFK_BARREL_DEATH,
    SFK_DEFEAT,

    SRD_THEME,
    SRD_FINAL_HIT,
    SRD_LASER_BURST,
    SRD_LASER_PREPARE,
    SRD_ORB,
    SRD_SWORD_BURST,
    SRD_SWORD_WALL,
    SRD_TELEPORT,

    BALDUR_BLOCK_SOUND,

    SOUL_TOTEM,

    GAME_OVER_SONG,
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
    BALDUR,
    ENEMY_ATTACK,
    ENTITY_BLOCK_TOP,
    ENTITY_BLOCK_BOTTOM,
    ENTITY_BLOCK_LEFT,
    ENTITY_BLOCK_RIGHT,
    ENTITY_BLOCK_BOSS_ACTIVATOR,
    ENTITY_ACTION_AREA,
    SPIKE,
    SCREEN_TRANSITION,
    LEVEL_TRANSITION,
    CURSOR,
    BUTTON,
    BARREL,
    TOTEM,
    RADIANCE,
};

enum HDirection
{
    H_LEFT = 1,
    H_RIGHT = 5,
};

enum Direction
{
    UP = 1,
    DOWN = 3,
    LEFT = 9,
    RIGHT = 27,
};

enum Orientation
{
    VERTICAL,
    HORIZONTAL,
};

enum Boss
{
    FALSE_KNIGHT,
    MANTIS_LORDS,
};

const float KNOCKBACK_SPEED = 128.0f;
const float KNOCKBACK_UP_SPEED = -196.0f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

const float LAYER_BG = 0.99f;
const float LAYER_LIGHT = 0.95f;
const float LAYER_GATE = 0.94f;
const float LAYER_TOTEM = 0.93f;
const float LAYER_RADIANCE = 0.92f;
const float LAYER_FG = 0.90f;
const float LAYER_ENEMY = 0.85f;
const float LAYER_PLAYER = 0.80f;
const float LAYER_HEAL = 0.79f;
const float LAYER_ENEMY_ATTACK = 0.75f;
const float LAYER_BOSS = 0.70f;
const float LAYER_BOSS_ATTACK = 0.65f;
const float LAYER_ATTACK = 0.60f;
const float LAYER_FIREBALL = 0.55f;
const float LAYER_HUD_BG = 0.50f;
const float LAYER_HUD_FG = 0.45f;
const float LAYER_TUTORIAL = 0.40;
const float LAYER_TRANSITION_SCREEN = 0.35f;
const float LAYER_TITLESCREEN = 0.30f;
const float LAYER_PAUSE_SCREEN = 0.25f;
const float LAYER_BUTTON = 0.20f;
const float LAYER_CURSOR = 0.15f;

inline bool Collision(Rect *ra, Rect *rb)
{
    bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right());
    bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom());

    return overlapX && overlapY;
}

#endif
