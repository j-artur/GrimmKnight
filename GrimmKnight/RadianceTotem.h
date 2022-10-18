#ifndef _RANDIANCETOTEM_H_
#define _RANDIANCETOTEM_H_

#include "Object.h"
#include "Sprite.h"

class RadianceTotem : public Object
{
  private:
    Sprite *sprite;
    Sprite *interactTutorial;

    bool close = false;
    float closeTime = 0.0f;
    float maxCloseTime = 0.25;

    bool ready = false;

  public:
    RadianceTotem(int x, int y);
    ~RadianceTotem();

    bool Ready();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
