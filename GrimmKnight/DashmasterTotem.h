#ifndef _DASHMASTER_TOTEM_H_
#define _DASHMASTER_TOTEM_H_

#include "Animation.h"
#include "Object.h"

class DashmasterTotem : public Object
{
  private:
    Sprite *sprite;
    Sprite *interactTutorial;

    bool close = false;
    float closeTime = 0.0f;
    float maxCloseTime = 0.25;

  public:
    DashmasterTotem(int x, int y);
    ~DashmasterTotem();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
