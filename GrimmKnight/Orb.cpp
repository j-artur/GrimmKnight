#include "Orb.h"
#include "GrimmKnight.h"

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
        GrimmKnight::scene->Delete(this, MOVING);
}

void Orb::Update()
{
    Rotate(900.0f * gameTime);

    acceleration = Vector(atan2((y - GrimmKnight::player->Y()) / 2.0f, x - GrimmKnight::player->X()) * 180 / M_PI,
                          640.0f * gameTime);
    speed.Add(acceleration);

    if (orbSpawnCd.Up() && !spawned)
    {
        spawned = true;
        speed =
            Vector(atan2(y - GrimmKnight::player->Y(), x - GrimmKnight::player->X()) * 180 / M_PI, 320.0f * gameTime);
        BBox(new Circle(16.0f));
        BBox()->MoveTo(x, y);
    }

    if (orbDeleteCd.Up())
        GrimmKnight::scene->Delete();

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
        float f = 5.0f - 4.0f * orbSpawnCd.Ratio();
        sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, orbSpawnCd.Ratio(), -rotation, {f, f, f, 1.0f});
    }
    else
        sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, -rotation);
}
