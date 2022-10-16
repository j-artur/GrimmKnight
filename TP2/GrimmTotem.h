#ifndef _GRIMM_TOTEM_H_
#define _GRIMM_TOTEM_H_

#include "Animation.h"
#include "Object.h"

class GrimmTotem : public Object
{
  private:
    TileSet *tileSet;
    Animation *anim;
    Sprite *interactTutorial;

    bool close = false;
    float closeTime = 0.0f;
    float maxCloseTime = 0.25;

  public:
    GrimmTotem(int x, int y);
    ~GrimmTotem();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
