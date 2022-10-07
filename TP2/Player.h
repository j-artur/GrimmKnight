
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Vector.h"
#include "Wall.h"

enum PlayerState
{
    STILL = 1,
    WALKING = 2,
    JUMPING = 4,
    FALLING = 8,
    ATTACKING = 16,
    CASTING = 32,
    DASHING = 64,
    HURTING = 128,
    DYING = 256,
    RESPAWNING = 512,
};

const char *PlayerStateToString(PlayerState state);

enum Direction
{
    LEFT = 1,
    RIGHT = 5,
};

const char *DirectionToString(Direction direction);

enum AttackDirection
{
    ATK_UP = 1,
    ATK_DOWN = 3,
    ATK_LEFT = 9,
    ATK_RIGHT = 27,
};

const char *AttackDirectionToString(AttackDirection direction);

class Player : public Object
{
  private:
    TileSet *tileSet;
    Animation *animation;
    Sprite *light;
    PlayerState state = FALLING;
    Direction direction = RIGHT;
    AttackDirection attackDirection = ATK_RIGHT;
    Cooldown attackCd{0.7f};
    Cooldown attackAnimCd{0.1f};
    Cooldown fireballCd{0.5f};
    Cooldown dashAnimCd{0.25f};
    Cooldown dashCd{0.9f};

    float oldTop;
    float oldBottom;
    float oldLeft;
    float oldRight;

    bool dashKeyCtrl = false;
    bool dashGroundCtrl = false;

    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    float walkingSpeed = 288.0f;
    float jumpingSpeed = -464.0f;
    float gravity = 768.0f;
    float dashSpeed = 720.0f;

  public:
    bool canMove = true;

    Player();
    ~Player();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
