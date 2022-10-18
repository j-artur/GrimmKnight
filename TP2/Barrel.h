#ifndef _BARREL_H_
#define _BARREL_H_

#include "Animation.h"
#include "Object.h"
#include "Player.h"
#include "TileSet.h"
#include "Types.h"
#include <random>

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

class Barrel : public Entity
{
  private:
    Sprite *sprite;

    mt19937 rng;
    uniform_int_distribution<int> posX;

    Cooldown barrelCd{2.0f};
    Cooldown hurtCd{0.25f};

    float gravity = 400.0f;
    float Yspeed = 100.0f;
    float Xspeed = 0.0f;

    bool deathCtrl = true;

  public:
    Barrel(Sprite *sp);
    ~Barrel();

    bool TakeDamage(uint damage, Direction dir);

    void Update();
    void Draw();
};

inline void Barrel::Draw()
{
    if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        sprite->Draw(x, y, LAYER_ENEMY, scale, rotation, {f, f, f, 1.0f});
    }
    else
        sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, rotation);
}

#endif
