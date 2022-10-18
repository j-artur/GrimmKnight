#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "Animation.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"
#include "Util.h"
#include <vector>

class Attack : public Object
{
  private:
    Animation *anim;
    Player *player;
    Direction direction;
    std::vector<Object *> objectsHit;

  public:
    Attack(TileSet *tileSet, Player *player, HDirection dir, Direction atkDir);
    ~Attack();

    Direction Dir();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

inline void Attack::Draw()
{
    anim->Draw(round(x), round(y), LAYER_ATTACK);
}

inline Direction Attack::Dir()
{
    return direction;
}

#endif
