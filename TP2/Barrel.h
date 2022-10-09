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

class Barrel : public Object
{
private:
    Sprite* sprite;

    mt19937 rng;
    uniform_int_distribution<int> posX;

    Cooldown barrelCd{ 2.0f };

    float speed = 600.0f;

public:
    Barrel(Sprite* sp);
    ~Barrel();

    void Update();
    void Draw();
};

inline void Barrel::Draw()
{
    sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK);
}

#endif
