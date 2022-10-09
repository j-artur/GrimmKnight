#include "Barrel.h"
#include "TP2.h"
#include "Util.h"

Barrel::Barrel(Sprite* sp)
{
    type = ENEMY_ATTACK;
    sprite = sp;

    random_device rd;
    rng.seed(rd());
    posX = uniform_int_distribution<int>(50, window->Width() - 50);

    barrelCd.Restart();

    MoveTo(posX(rng), -32.0f);

    int width = 64;
    int height = 64;

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
}

// ---------------------------------------------------------------------------------

Barrel::~Barrel()
{
    delete sprite;
}

// ---------------------------------------------------------------------------------

void Barrel::Update()
{
    Translate(0, speed * gameTime);

    Rotate(0.5 * gameTime);

    if (barrelCd.Up())
    {
        TP2::scene->Delete(this, MOVING);
    }
}
