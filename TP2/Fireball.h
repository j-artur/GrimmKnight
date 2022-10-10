#ifndef _FIREBALL_H_
#define _FIREBALL_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"
#include "Util.h"

class Fireball : public Object
{
  private:
    TileSet *ts;
    Animation *anim;
    Cooldown fireballCd{2.0f};
    HDirection direction;

    float speed = 800.0f;
    float distance = 14.0f;

  public:
    Fireball(Player *player, HDirection direction);
    ~Fireball();

    Direction Dir();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

inline void Fireball::Draw()
{
    anim->Draw(round(x), round(y), LAYER_FIREBALL);
}

inline Direction Fireball::Dir()
{
    return direction == H_LEFT ? LEFT : RIGHT;
}

#endif
