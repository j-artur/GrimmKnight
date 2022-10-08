
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
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

class Player : public Entity
{
  private:
    TileSet *tileSet;
    Animation *animation;
    Sprite *light;
    TileSet *attackTileSet;

    PlayerState state = FALLING;
    Direction direction = RIGHT;
    AttackDirection attackDirection = ATK_RIGHT;

    int mana = 0;

    Cooldown attackCd{0.5f};
    Cooldown attackAnimCd{0.2f};
    Cooldown fireballCd{0.4f};
    Cooldown fireballAnimCd{0.2f};
    Cooldown dashAnimCd{0.25f};
    Cooldown dashCd{0.9f};
    Cooldown hurtCd{0.9f};
    Cooldown hurtAnimCd{0.2f};
    Cooldown knockbackCd{0.15f};

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
    Player();
    ~Player();

    int Hp();
    int Mana();

    bool TakeDamage(uint damage, AttackDirection dir);
    void AddMana();
    void AddCooldowns(float dt);
    void RefreshCooldowns();
    void Knockback();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

inline int Player::Hp()
{
    return hp;
}

inline int Player::Mana()
{
    return mana;
}

#endif
