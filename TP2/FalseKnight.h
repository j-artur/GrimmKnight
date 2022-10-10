#ifndef _FALSE_KNIGHT_H_
#define _FALSE_KNIGHT_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Mace.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Wall.h"
#include <random>

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

enum FK_State
{
    FK_IDLE,
    FK_LEAP,
    FK_SLAM,
    FK_BLUDGEON,
    FK_RAGE,
    FK_STUN,
    FK_DEAD,
};

enum FK_Animation
{
    FK_LEAPING = 1,
    FK_PREP_SLAM = 2,
    FK_SLAMMING = 4,
    FK_BLUDGEONING = 8,
    FK_RAGING = 16,
};

enum FK_JumpTo
{
    J_PLAYER,
    J_MIDDLE,
};

class FalseKnight : public Entity
{
  private:
    TileSet *tileSet;
    TileSet *shockwaveTileSet;
    Sprite *barrelSprite;
    Animation *animation;

    mt19937 rng;
    uniform_int_distribution<int> cd;
    uniform_int_distribution<int> move;

    FK_State state = FK_IDLE;
    HDirection direction = H_LEFT;
    Mace *mace;

    uint armorHealth = 65;
    uint currentArmorHealth = armorHealth;
    uint headHealth = 40;
    uint attackCd = 2;
    uint nextMove;

    Cooldown slamCd{2.0f};
    Cooldown prepSlamCd{1.2f};
    Cooldown jumpCd{2.0f};
    Cooldown rageCd{3.2f};
    Cooldown attackRageCd{0.8f};
    Cooldown betweenAttacksCd{5.0f};
    Cooldown hurtCd{ 0.3f };
    Cooldown headOutCd{ 1.0f };
    Cooldown stunCd{ 3.0f };

    float directionMult = 1.0f;
    float attackDirection = 1.0f;
    float ySpeed = 0.0f;
    float xSpeed = 0.0f;
    float leapSpeed = -768.0f;
    float bludgeonSpeed = -537.6f;
    float gravity = 768.0f;

    bool strikeCtrl = true;
    bool voiceCtrl = true;
    bool isJumping = false;
    bool isAttacking = false;
    bool isStunned = false;
    bool rageStarted = false;
    bool spawnedShockwave = false;
    bool spawnedBarrels = false;
    bool canKill = false;

  public:
    FalseKnight();
    ~FalseKnight();

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
    void OnCollision(Object *other);
    void JumpTo(FK_JumpTo jumpTo);
};

#endif
