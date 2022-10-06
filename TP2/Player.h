
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
    IDLE_RIGHT,
    IDLE_LEFT,
    WALK_RIGHT,
    WALK_LEFT
};

enum AttackDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Facing
{
    F_LEFT,
    F_RIGHT
};

class Player : public Object
{
  private:
    TileSet *tileSet;
    Animation *animation;
    Sprite *light;
    PlayerState state;
    Cooldown attackCd{1.1f};
    Cooldown fireballCd{0.5f};
    Cooldown dashingCd{0.25f};
    Cooldown dashCd{0.75f};
    AttackDirection attackDirection = RIGHT;
    Facing facing = F_RIGHT;

    float oldTop;
    float oldBottom;
    float oldLeft;
    float oldRight;

    bool jumping = false;
    bool standing = false;
    float jumpingAmount = 0.0f;
    bool dying = false;
    bool attacking = false;
    bool fireballing = false;
    bool dashing = false;
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
