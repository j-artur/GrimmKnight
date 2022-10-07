
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"

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

class Player : public Object
{
private:
    TileSet *tileSet;
    Animation *animation;
    Sprite *light;
    TileSet *attackTileSet;

    PlayerState state = FALLING;
    Direction direction = RIGHT;
    AttackDirection attackDirection = ATK_RIGHT;

    uint mana = 0;

    Cooldown attackCd{0.7f};
    Cooldown attackAnimCd{0.2f};
    Cooldown fireballCd{0.5f};
    Cooldown dashAnimCd{0.25f};
    Cooldown dashCd{0.9f};

    bool jumpKeyCtrl = true;
    bool attackKeyCtrl = true;
    bool fireballKeyCtrl = true;
    bool dashKeyCtrl = true;
    bool dashGroundCtrl = true;

    float oldTop;
    float oldBottom;
    float oldLeft;
    float oldRight;

    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    float walkingSpeed = 288.0f;
    float jumpingSpeed = -464.0f;
    float gravity = 768.0f;
    float dashSpeed = 720.0f;

    bool HasMana();
    void UseMana();

  public:
    bool canMove = true;

    Player();
    ~Player();

    void AddMana();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

bool inline Player::HasMana() {
    return mana >= 3;
}

void inline Player::UseMana() {
    mana -= 3;
}

void inline Player::AddMana() {
    if (mana < 9)
        mana++;
}

#endif
