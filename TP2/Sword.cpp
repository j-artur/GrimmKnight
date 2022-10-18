#include "Sword.h"
#include "TP2.h"

Sword::Sword(Sprite *sprite, float angle)
{
    type = ENEMY_ATTACK;

    this->direction = Vector(angle, 24.0f);
    this->sprite = sprite;

    RotateTo(angle);

    bb = new Mixed();

    for (int i = 0; i < 8; i++)
    {
        circles[i] = new Circle(10.0f);
        bb->Insert(circles[i]);
    }

    BBox(bb);
}

Sword::~Sword()
{
}

void Sword::Update()
{

    if (swordSpawnCd.Up() && !spawned)
    {
        spawned = true;
        for (int i = 0; i < 8; i++)
            circles[i]->MoveTo(x + direction.XComponent() * i, y - direction.YComponent() * i);
    }

    if (spawned)
    {
        this->direction.ScaleTo(600.0f);
        Translate(direction.XComponent() * gameTime, -direction.YComponent() * gameTime);
    }

    if (swordDeleteCd.Up())
        TP2::scene->Delete();

    swordDeleteCd.Add(gameTime);
    swordSpawnCd.Add(gameTime);
}

void Sword::Draw()
{
    if (swordSpawnCd.Down())
    {

        float f = 100.0f - 99.0f * swordSpawnCd.Ratio();
        sprite->Draw(round(x + direction.XComponent() / 8.0f), round(y - direction.YComponent() / 8.0f),
                     LAYER_BOSS_ATTACK, swordSpawnCd.Ratio(), -rotation, {f, f, f, 1.0f});
    }
    else
        sprite->Draw(round(x + direction.XComponent() / 8.0f), round(y - direction.YComponent() / 8.0f),
                     LAYER_BOSS_ATTACK, 1.0f, -rotation);
}
