#include "Beam.h"
#include "TP2.h"

Beam::Beam(TileSet *tileSet, float angle)
{
    type = ENEMY_ATTACK;

    animation = new Animation(tileSet, 0.75f, true);

    this->direction = Vector(angle, 40.0f);
    bb = new Mixed();

    RotateTo(angle);

    for (int i = 0; i < 20; i++)
    {
        circles[i] = new Circle(20.0f);

        bb->Insert(circles[i]);
    }

    BBox(bb);
}

Beam::~Beam()
{
    delete animation;
}

void Beam::Update()
{
    if (beamSpawnCd.Up() && spawnCtrl)
    {
        spawnCtrl = false;
        TP2::audio->Play(SRD_LASER_BURST);
        for (int i = 0; i < 20; i++)
            circles[i]->MoveTo(x + direction.XComponent() * i, y - direction.YComponent() * i);
    }

    if (beamDeactivateCd.Up() && deactivateCtrl)
    {
        deactivateCtrl = false;
        for (int i = 0; i < 20; i++)
            ((Mixed *)BBox())->Remove(circles[i]);
    }

    if (beamDeleteCd.Up())
    {
        TP2::scene->Delete();
    }

    beamSpawnCd.Add(gameTime);
    beamDeactivateCd.Add(gameTime);
    beamDeleteCd.Add(gameTime);
    animation->NextFrame();
}

void Beam::Draw()
{
    animation->Draw(round(x + direction.XComponent()), round(y - direction.YComponent()), LAYER_BOSS_ATTACK, scale,
                    -rotation);
}
