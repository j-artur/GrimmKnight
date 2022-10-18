#ifndef _FALSE_KNIGHT_HEAD_H_
#define _FALSE_KNIGHT_HEAD_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"

class FalseKnightHead : public Object
{
  private:
    TileSet *tileSet;
    Animation *animation;

    HDirection direction = H_LEFT;
    Cooldown hurtCd{0.20f};

  public:
    FalseKnightHead(TileSet *ts);
    ~FalseKnightHead();

    void TakeDamage();

    void setDirection(HDirection d);

    void Update();
    void Draw();
};

#endif
