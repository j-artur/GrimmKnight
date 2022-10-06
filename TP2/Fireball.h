#ifndef _FIREBALL_H_
#define _FIREBALL_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"

class Fireball : public Object
{
  private:
    TileSet *ts;
    Animation *anim;
    Cooldown fireballCd{2.0f};

    float speed = 800.0f;
    float distance;

  public:
    Fireball(Player *player, Direction direction);
    ~Fireball();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

inline void Fireball::Draw()
{
    anim->Draw(x, y, z);
}

#endif
