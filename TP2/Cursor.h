#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "Object.h"
#include "Sprite.h"

class Cursor : public Object
{
  private:
    Sprite *sprite;

  public:
    Cursor();
    ~Cursor();

    void Update();
    void Draw();
};

#endif
