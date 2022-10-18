#ifndef _SPIKE_H_
#define _SPIKE_H_

#include "Object.h"
#include "Util.h"

class Spike : public Object
{
  private:
    float absX;
    float absY;
    Direction dir;

  public:
    Spike(int x, int y, int width, int height, Direction dir);

    Direction Dir();

    void Update(){};
    void Draw(){};
};

inline Direction Spike::Dir()
{
    return dir;
}

#endif
