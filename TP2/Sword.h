#ifndef _SWORD_H_
#define _SWORD_H_

#include "Animation.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Cooldown.h"
#include "Vector.h"

class Sword : public Object
{
private:
    Sprite* sprite;

    Mixed* bb;
    Circle* circles[12];
    Vector direction;

    Cooldown swordDeleteCd{ 2.0f, false };
    Cooldown swordSpawnCd{ 0.5f, false };

    bool spawned = false;

public:
    Sword(Vector direction, Sprite* sprite);
    ~Sword();

    void Update();
    void Draw();

};

#endif