#ifndef _WALL_H_
#define _WALL_H_

#include "Object.h"
#include "Util.h"

class Wall : public Object
{
  public:
    Wall(int x1, int y1, int width, int height);

    void Update();
    void Draw();
};

#endif
