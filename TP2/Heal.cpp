#include "Heal.h"
#include "TP2.h"
#include "Util.h"

Heal::Heal()
{
    tileSet = new TileSet("Resources/Heal.png", 1, 3);
    animation = new Animation(tileSet, 0.1, false);
}

Heal::~Heal()
{
    delete tileSet;
    delete animation;
}

void Heal::Update()
{
    if (animation->Inactive())
        TP2::scene->Delete();
    else
        animation->NextFrame();
}

void Heal::Draw()
{
    animation->Draw(TP2::player->X(), TP2::player->Y(), LAYER_HEAL);
}
