#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "Animation.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"

class Attack : public Object
{
  private:
    Animation *anim;
    Player *player;
    AttackDirection direction;

  public:
    Attack(TileSet *tileSet, Player *player, AttackDirection direction);
    ~Attack();

    void Update();
    void Draw();
};

inline void Attack::Draw()
{
    anim->Draw(x, y, Layer::UPPER);
}

#endif
