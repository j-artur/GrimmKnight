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

enum FK_State
{
    FK_IDLE = 1,
    FK_LEAP = 2,
    FK_SLAM = 4,
    FK_BLUDGEON = 8,
    FK_RAGE = 16,
    FK_STUN = 32,
    FK_DEAD = 64,
};

enum FK_Animation
{
    FK_LEAPING,
    FK_PREP_SLAM,
    FK_SLAMMING,
    FK_BLUDGEONING,
    FK_RAGING
};

class FalseKnight : public Entity
{
  private:
    TileSet *tileSet;
    TileSet *shockwaveTileSet;
    Animation *animation;

    FK_State state = FK_IDLE;
    Direction direction = RIGHT;
    Mace *mace;

    uint armorHealth = 65;
    uint headHealth = 40;

    Cooldown slamCd{2.0f};
    Cooldown prepSlamCd{1.2f};

    float directionMult = 1.0f; // going to use to invert x axis on left/right attacks
    float ySpeed = 0.0f;
    float jumpingSpeed = -464.0f;
    float gravity = 768.0f;

    bool keyCtrl = true;
    bool spawnedShockwave = false;
    bool canKill = false;

  public:
    FalseKnight();
    ~FalseKnight();

    bool TakeDamage(uint damage, AttackDirection dir);

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
