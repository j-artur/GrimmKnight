
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

    Cooldown attackCd{0.5f};
    Cooldown attackAnimCd{0.2f};

    Cooldown hurtCd{0.9f};
    Cooldown hurtAnimCd{0.2f};
    Cooldown knockbackCd{0.15f};
    Cooldown knockbackUpCd{0.2f};

    bool jumpKeyCtrl = true;
    bool attackKeyCtrl = true;

    int mana = 0;
    bool fireball = false;
    Cooldown fireballCd{0.4f};
    Cooldown fireballAnimCd{0.2f};
    bool fireballKeyCtrl = true;

    bool dash = false;
    Cooldown dashCd{0.9f};
    Cooldown dashAnimCd{0.25f};
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

    void TakeKnockback();

  public:
    Player();
    ~Player();

    int Hp();
    int Mana();

    bool HasFireball();
    void LearnFireball();

    bool HasDash();
    void LearnDash();

    bool TakeDamage(uint damage, AttackDirection dir);
    void AddMana();
    void AddCooldowns(float dt);
    void RefreshCooldowns();
    void Knockback();

    void State(PlayerState state);
    void Dir(Direction dir);
    void UpdateAnimation();

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

inline bool Player::HasFireball()
{
    return fireball;
}

inline void Player::LearnFireball()
{
    fireball = true;
}

inline bool Player::HasDash()
{
    return dash;
}

inline void Player::LearnDash()
{
    dash = true;
}

inline void Player::State(PlayerState state)
{
    this->state = state;
}

inline void Player::Dir(Direction dir)
{
    direction = dir;
}

#endif
