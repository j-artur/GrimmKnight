#ifndef _SPIKE_H_
#define _SPIKE_H_

#include "Object.h"
#include "Util.h"

class Spike : public Object
{
  private:
    float absX;
    float absY;
    AttackDirection dir;

  public:
    Spike(int x, int y, int width, int height, AttackDirection dir);

    AttackDirection Dir();

    void Update(){};
    void Draw(){};
};

inline AttackDirection Spike::Dir()
{
    return dir;
}

#endif
