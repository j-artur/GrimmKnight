#include "Orb.h"
#include "TP2.h"

Orb::Orb(Sprite *sprite)
{
    type = ENEMY_ATTACK;

    this->sprite = sprite;

    maxSpeed = 640.0f;
}

Orb::~Orb()
{
}

void Orb::OnCollision(Object *other)
{
    if (other->Type() == WALL_TOP)
        TP2::scene->Delete(this, MOVING);
}

void Orb::Update()
{
    Rotate(900.0f * gameTime);

    acceleration = Vector(atan2((y - TP2::player->Y()) / 2.0f, x - TP2::player->X()) * 180 / M_PI, 640.0f * gameTime);
    speed.Add(acceleration);

    if (orbSpawnCd.Up() && !spawned)
    {
        spawned = true;
        speed = Vector(atan2(y - TP2::player->Y(), x - TP2::player->X()) * 180 / M_PI, 320.0f * gameTime);
        BBox(new Circle(16.0f));
        BBox()->MoveTo(x, y);
    }

    if (orbDeleteCd.Up())
        TP2::scene->Delete();

    if (speed.Magnitude() > maxSpeed)
        speed.ScaleTo(maxSpeed);

    if (spawned)
        Translate(-speed.XComponent() * gameTime, -speed.YComponent() * gameTime);

    orbSpawnCd.Add(gameTime);
    orbDeleteCd.Add(gameTime);
}

void Orb::Draw()
{
    if (orbSpawnCd.Down())
    {
        float f = 100.0f - 99.0f * orbSpawnCd.Ratio();
        sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, orbSpawnCd.Ratio(), -rotation, {f, f, f, 1.0f});
    }
    else
        sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, -rotation);
}
