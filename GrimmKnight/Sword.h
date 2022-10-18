#ifndef _SWORD_H_
#define _SWORD_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Vector.h"

class Sword : public Object
{
  private:
    Sprite *sprite;

    Mixed *bb;
    Circle *circles[10];
    Vector direction;

    Cooldown swordDeleteCd{3.0f, false};
    Cooldown swordSpawnCd{1.0f, false};

    bool spawned = false;

  public:
    Sword(Sprite *sprite, float angle);
    ~Sword();

    void Update();
    void Draw();
};

#endif
