#ifndef _GATE_H_
#define _GATE_H_

#include "Cooldown.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"

class Gate : public Object
{
  private:
    Sprite *sprite = nullptr;
    HDirection direction;
    bool opened = true;
    bool moving = false;

  public:
    Gate(Image *img, HDirection direction, int x, int y);
    ~Gate();

    void Open();
    void Close();

    void Update();
    void Draw();
};

#endif
