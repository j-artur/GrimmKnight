#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Object.h"
#include "Util.h"

class Entity : public Object
{
  private:
    int hp;

  public:
    virtual void TakeDamage(uint damage, AttackDirection dir) = 0;

    int Hp();
};

inline int Entity::Hp()
{
    return hp;
}

#endif
