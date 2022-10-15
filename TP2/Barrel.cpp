#include "Barrel.h"
#include "TP2.h"
#include "Util.h"

Barrel::Barrel(Sprite *sp)
{
    type = ENEMY;
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

Barrel::~Barrel()
{
}

bool Barrel::TakeDamage(uint damage, Direction dir)
{
    switch (dir)
    {
    case UP:
    case DOWN:
        break;
    case LEFT:
        Xspeed = -4000.0f;
        type = BARREL;
        break;
    case RIGHT:
        Xspeed = 4000.0f;
        type = BARREL;
        break;
    default:
        break;
    }

    return false;
}

void Barrel::Update()
{

    Rotate(1.0f);

    if (barrelCd.Up())
    {
        TP2::scene->Delete();
        TP2::audio->Play(SFK_BARREL_DEATH);
    }

    Yspeed += gravity * gameTime;

    Translate(Xspeed * gameTime, Yspeed * gameTime);
    barrelCd.Add(gameTime);
}
