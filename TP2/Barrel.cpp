#include "Barrel.h"
#include "TP2.h"
#include "Util.h"

Barrel::Barrel(Sprite *sp)
{
    type = ENEMY;
    sprite = sp;

    random_device rd;
    rng.seed(rd());
    posX = uniform_int_distribution<int>(256, 1152);

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
    hurtCd.Restart();
    switch (dir)
    {
    case UP:
    case DOWN:
        break;
    case LEFT:
        Xspeed = -1000.0f;
        type = BARREL;
        break;
    case RIGHT:
        Xspeed = 1000.0f;
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

    if (type == BARREL)
        Translate(Xspeed * gameTime, 0);
    else
        Translate(0, Yspeed * gameTime);

    Yspeed += gravity * gameTime;

    barrelCd.Add(gameTime);
    hurtCd.Add(gameTime);
}
