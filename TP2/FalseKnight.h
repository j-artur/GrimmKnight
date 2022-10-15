#ifndef _FALSE_KNIGHT_H_
#define _FALSE_KNIGHT_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Wall.h"
#include "Shockwave.h"
#include "FalseKnightHead.h"
#include <random>

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

enum FK_State
{
    FK_INACTIVE,
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
    FK_LEAPING = 2,
    FK_PREP_SLAM = 4,
    FK_SLAMMING = 8,
    FK_BLUDGEONING_J = 16,
    FK_BLUDGEONING = 32,
    FK_RAGING = 64,
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
    TileSet* headTileSet;
    TileSet *shockwaveTileSet;
    Sprite *barrelSprite;
    Animation *animation;

    FalseKnightHead* head;

    Rect* self;
    Rect* horn;
    Rect* hit;
    Rect* hand;
    Circle* mace;
    Mixed* bb;

    mt19937 rng;
    uniform_int_distribution<int> cd;
    uniform_int_distribution<int> move;

    FK_State state = FK_INACTIVE;
    HDirection direction = H_LEFT;
    HDirection attackDirection = H_LEFT;
    HDirection stunDirection = H_LEFT;
    Shockwave* shockwave;

    uint armorHealth = 65;
    uint headHealth = 40;
    uint currentArmorHealth = armorHealth;
    uint nextMove = 2;
    uint oldMove = 2;

    Cooldown slamCd{1.8f};
    Cooldown prepSlamCd{1.2f};
    Cooldown jumpCd{2.0f};
    Cooldown bludgeonAttackCd{ 0.2f };
    Cooldown rageCd{3.2f};
    Cooldown attackRageCd{0.4f};
    Cooldown betweenAttacksCd{2.5f};
    Cooldown hurtCd{ 0.3f };
    Cooldown headOutCd{ 0.5f };
    Cooldown stunCd{ 6.0f };

    float directionMult = 1.0f;
    float ySpeed = 0.0f;
    float xSpeed = 0.0f;
    float jumpSpeed = -468.0f;
    float gravity = 468.0f;

    bool strikeCtrl = true;
    bool voiceCtrl = true;
    bool voice2Ctrl = true;
    bool isJumping = false;
    bool isAttacking = false;
    bool isStunned = false;
    bool rageStarted = false;
    bool spawnedShockwave = false;
    bool spawnedBarrels = false;
    bool animRestarted = false;
    bool canKill = false;
    bool active = false;

  public:
    FalseKnight(int iX, int iY);
    ~FalseKnight();

    bool TakeDamage(uint damage, Direction dir);

    void Activate();

    void Update();
    void Draw();
    void OnCollision(Object *other);
    void JumpTo(FK_JumpTo jumpTo);
    void DraftMove();
    bool Active();
};

inline bool FalseKnight::Active()
{
    return active;
}

#endif
