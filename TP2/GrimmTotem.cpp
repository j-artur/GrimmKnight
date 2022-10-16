#include "GrimmTotem.h"
#include "Player.h"
#include "TP2.h"
#include "Util.h"

GrimmTotem::GrimmTotem(int iX, int iY)
{
    tileSet = new TileSet("Resources/Grimm.png", 1, 2);
    anim = new Animation(tileSet, 0.0f, false);

    uint seqEnabled[1] = {0};
    uint seqDisabled[1] = {1};

    anim->Add(0, seqEnabled, 1);
    anim->Add(1, seqDisabled, 1);

    anim->Select(TP2::player->HasFireball());

    BBox(new Rect(-32.0f, 50.0f, 32.0f, 50.0f));

    MoveTo(iX * 32.0f + 26.0f, iY * 32.0f - 50.0f);
}

GrimmTotem::~GrimmTotem()
{
    delete anim;
    delete tileSet;
}

void GrimmTotem::Update()
{
    anim->NextFrame();
}

void GrimmTotem::Draw()
{
    anim->Draw(X(), Y(), LAYER_TOTEM);
}

void GrimmTotem::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER && !TP2::player->HasFireball() && window->KeyPress(VK_UP))
    {
        TP2::player->State(STILL);
        TP2::GetFireball();
        anim->Select(1);
    }
}
