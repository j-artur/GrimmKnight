#ifndef _TIKTIK_H_
#define _TIKTIK_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Util.h"

enum TiktikState
{
    TIKTIK_WALKING = 1,
    TIKTIK_HURTING = 2,
    TIKTIK_DEAD = 8,
};

class Tiktik : public Entity
{
  private:
    Animation *animation;
    TiktikState state = TIKTIK_WALKING;
    HDirection direction = H_RIGHT;

    Cooldown hurtCd{0.3f};
    Cooldown dieCd{2.0f};

    float walkSpeed = 64.0f;
    float gravity = 768.0f;
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;

    bool audioCtrl = true;

  public:
    Tiktik(TileSet *tileSet, int x, int y);
    ~Tiktik();

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
