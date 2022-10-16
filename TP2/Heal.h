#ifndef _HEAL_H_
#define _HEAL_H_

#include "Animation.h"
#include "Object.h"

class Heal : public Object
{
  private:
    TileSet *tileSet;
    Animation *animation;

  public:
    Heal();
    ~Heal();

    void Update();
    void Draw();
};

#endif
