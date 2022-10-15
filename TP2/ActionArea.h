#ifndef _ACTION_AREA_H_
#define _ACTION_AREA_H_

#include "Object.h"
#include "TP2.h"
#include "Types.h"
#include "Util.h"

class ActionArea : public Object
{
  private:
    bool playerInside = false;
    float x1;
    float x2;

    HDirection playerDirection = H_LEFT;
    Player *player = TP2::player;

  public:
    ActionArea(float x1, float y1, float x2, float y2);
    ~ActionArea();

    void Update();
    void Draw();
    void OnCollision(Object *other);

    bool IsPlayerInside();

    HDirection PlayerDirection();
};

inline void ActionArea::Draw()
{
}

inline bool ActionArea::IsPlayerInside()
{
    return playerInside;
}

inline HDirection ActionArea::PlayerDirection()
{
    return playerDirection;
}

#endif
