#ifndef _WANDERING_HUSK_H_
#define _WANDERING_HUSK_H_

#include "ActionArea.h"
#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Util.h"

enum WH_State
{
    WH_WALKING = 1,
    WH_PRE_RUNNING = 2,
    WH_RUNNING = 4,
    WH_HURTING = 8,
    WH_DEAD = 16,
};

class WanderingHusk : public Entity
{
  private:
    Animation *animation;
    WH_State state = WH_WALKING;
    HDirection direction = H_RIGHT;
    uint voiceId;

    Cooldown audioCd{1.0f};
    Cooldown hurtCd{0.25f};
    Cooldown chargeRunCd{0.5f};
    Cooldown runCd{3.0f};
    Cooldown dieCd{2.0f};

    ActionArea *actionArea;

    bool playerInside = false;
    bool audioCtrl = true;
    bool runCtrl = true;

    float walkSpeed = 64.0f;
    float runningSpeed = 144.0f;
    float gravity = 768.0f;
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;

  public:
    WanderingHusk(TileSet *tileSet, int x, int y, uint voiceId);
    ~WanderingHusk();

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
