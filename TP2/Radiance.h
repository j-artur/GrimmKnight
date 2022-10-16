#ifndef _RADIANCE_H_
#define _RADIANCE_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Wall.h"
#include "Vector.h"
#include <random>

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

enum RD_State
{
    RD_IDLE,
    RD_SWORD_BURST,
    RD_SWORD_WALL,
    RD_SWORD_RAIN,
    RD_BEAM_WALL,
    RD_BEAM_BURST,
    RD_ORB
};

enum RD_Attack
{
    RD_SWORD_A = 20,
    RD_BEAM_A = 40
};

class Radiance : public Entity
{
private:
    TileSet* tileSet;
    Sprite* swordSprite;
    Animation* animation;

    Rect* head;

    mt19937 rng;
    uniform_int_distribution<int> move;
    uniform_int_distribution<int> angle;

    RD_State state = RD_IDLE;

    uint nextMove = 2;
    uint oldMove = 2;
    uint startingAngle = 0;

    Vector direction[6];
    Vector headDistance[6];

    Cooldown betweenAttacksCd{ 2.0f };
    Cooldown hurtCd{ 0.5f };

    float directionMult = 1.0f;

    bool active = false;
    bool isAttacking = false;

public:
    Radiance();
    ~Radiance();

    bool TakeDamage(uint damage, Direction dir);

    void DraftAngle(RD_Attack attack);
    void Update();
    void Draw();
    void DraftMove();
};

#endif
