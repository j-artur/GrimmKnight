#ifndef _MANA_TOTEM_H_
#define _MANA_TOTEM_H_

#include "Cooldown.h"
#include "Entity.h"
#include "Sprite.h"

class Totem : public Entity
{
  private:
    Sprite *sprite = nullptr;
    int mana;
    int maxMana;
    Cooldown hurtCd{0.25f};

  public:
    Totem(Sprite *sprite, int x, int y);

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
};

#endif
