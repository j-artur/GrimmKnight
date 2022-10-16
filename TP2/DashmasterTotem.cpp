#include "DashmasterTotem.h"
#include "Player.h"
#include "TP2.h"
#include "Util.h"

DashmasterTotem::DashmasterTotem(int iX, int iY)
{
    sprite = new Sprite("Resources/Dashmaster.png");

    BBox(new Rect(-64.0f, 96.0f, 64.0f, 96.0f));

    MoveTo(iX * 32.0f + 48.0f, iY * 32.0f - 96.0f);
}

DashmasterTotem::~DashmasterTotem()
{
    delete sprite;
}

void DashmasterTotem::Update()
{
}

void DashmasterTotem::Draw()
{
    sprite->Draw(X(), Y(), LAYER_TOTEM);
}

void DashmasterTotem::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER && !TP2::player->HasDash() && window->KeyPress(VK_UP))
    {
        TP2::player->State(STILL);
        TP2::GetDash();
    }
}
