#ifndef _DASHMASTER_TOTEM_H_
#define _DASHMASTER_TOTEM_H_

#include "Animation.h"
#include "Object.h"

class DashmasterTotem : public Object
{
  private:
    Sprite *sprite;

  public:
    DashmasterTotem(int x, int y);
    ~DashmasterTotem();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
