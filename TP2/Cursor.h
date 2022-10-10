#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "Object.h"

class Cursor : public Object
{
  public:
    Cursor();

    void Update();
    void Draw(){};
};

#endif
