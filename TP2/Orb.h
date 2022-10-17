#ifndef _ORB_H_
#define _ORB_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Vector.h"

class Orb : public Object
{
  private:
    Sprite *sprite;

    Vector speed;
    Vector acceleration;

    float maxSpeed;

    Cooldown orbDeleteCd{5.0f, false};
    Cooldown orbSpawnCd{1.5f, false};

    bool spawned = false;

  public:
    Orb(Sprite *sprite);
    ~Orb();

    void OnCollision(Object *other);

    void Update();
    void Draw();
};

#endif
