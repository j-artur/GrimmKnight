#include "DashmasterTotem.h"
#include "Player.h"
#include "TP2.h"
#include "Util.h"

DashmasterTotem::DashmasterTotem(int iX, int iY)
{
    sprite = new Sprite("Resources/Dashmaster.png");
    interactTutorial = new Sprite("Resources/TutorialInteract.png");

    BBox(new Rect(-64.0f, 96.0f, 64.0f, 96.0f));

    MoveTo(iX * 32.0f + 48.0f, iY * 32.0f - 96.0f);
}

DashmasterTotem::~DashmasterTotem()
{
    delete sprite;
}

void DashmasterTotem::Update()
{
    if (close)
    {
        closeTime += gameTime;
        if (closeTime > maxCloseTime)
            closeTime = maxCloseTime;
    }
    else
    {
        closeTime -= gameTime;
        if (closeTime < 0.0f)
            closeTime = 0.0f;
    }

    close = false;
}

void DashmasterTotem::Draw()
{
    sprite->Draw(X(), Y(), LAYER_TOTEM);
    if (!TP2::player->HasDash() && closeTime > 0.0f)
        interactTutorial->Draw(x, y + 160.0f, LAYER_TUTORIAL, closeTime / maxCloseTime, 0.0f,
                               {1.0f, 1.0f, 1.0f, closeTime / maxCloseTime});
}

void DashmasterTotem::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER)
    {
        close = true;
        if (!TP2::player->HasDash() && window->KeyPress(VK_UP))
        {
            TP2::player->State(STILL);
            TP2::GetDash();
        }
    }
}
