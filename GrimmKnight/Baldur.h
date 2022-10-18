#ifndef _BALDUR_H_
#define _BALDUR_H_

#include "ActionArea.h"
#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Wall.h"

enum BaldurState
{
    BALDUR_IDLE,
    BALDUR_BLOCK,
    BALDUR_DEAD,
};

class Baldur : public Entity
{
  private:
    TileSet *tileSet = nullptr;
    Animation *animation = nullptr;
    BaldurState state = BALDUR_IDLE;
    ActionArea *actionArea = nullptr;
    Cooldown deathCd{0.4f};
    Cooldown hurtCd{0.25f};

    Wall *rightWall = nullptr;
    Wall *topWall = nullptr;

    bool soundCtrl = true;

  public:
    Baldur(int x, int y);
    ~Baldur();

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
