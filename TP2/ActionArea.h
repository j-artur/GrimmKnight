#ifndef _ACTION_AREA_H_
#define _ACTION_AREA_H_

#include "Object.h"
#include "Types.h"
#include "Util.h"
#include "TP2.h"

class ActionArea : public Object
{
private:
    bool playerInside = false;

    Direction playerDirection = LEFT;
    Player* player = TP2::player;

public:
    ActionArea();
    ~ActionArea();

    void Update();
    void Draw();
    void OnCollision(Object* other);

    bool IsPlayerInside();
    
    Direction PlayerDirection();
};

inline void ActionArea::Draw()
{
}

inline bool ActionArea::IsPlayerInside()
{
    return playerInside;
}

inline Direction ActionArea::PlayerDirection()
{
    return playerDirection;
}


#endif
