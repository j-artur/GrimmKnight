#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Object.h"
#include "Util.h"

class Entity : public Object
{
  protected:
    int hp;

  public:
    virtual bool TakeDamage(uint damage, Direction dir) = 0;

    int Hp();
    bool Alive();
};

inline int Entity::Hp()
{
    return hp;
}

inline bool Entity::Alive()
{
    return hp > 0;
}

#endif
