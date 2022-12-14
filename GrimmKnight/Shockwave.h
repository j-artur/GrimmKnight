#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "Animation.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"

class Shockwave : public Object
{
  private:
    TileSet *ts;
    Animation *anim;

    float speed = 800.0f;

  public:
    Shockwave(Geometry *mace, HDirection direction, TileSet *tileset);
    ~Shockwave();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

inline void Shockwave::Draw()
{
    anim->Draw(round(x), round(y), LAYER_BOSS_ATTACK);
}

#endif
