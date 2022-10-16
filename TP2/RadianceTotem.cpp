#include "RadianceTotem.h"
#include "Player.h"
#include "TP2.h"
#include "Util.h"

RadianceTotem::RadianceTotem(int iX, int iY)
{
    sprite = new Sprite("Resources/RadianceTotem.png");
    interactTutorial = new Sprite("Resources/TutorialInteract.png");

    BBox(new Rect(-64.0f, 80.0f, 64.0f, 80.0f));

    MoveTo(iX * 32.0f + 48.0f, iY * 32.0f - 80.0f);
}

RadianceTotem::~RadianceTotem()
{
    delete sprite;
    delete interactTutorial;
}

bool RadianceTotem::Ready()
{
    return ready;
}

void RadianceTotem::Update()
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

void RadianceTotem::Draw()
{
    sprite->Draw(X(), Y(), LAYER_TOTEM);
    if (closeTime > 0.0f)
        interactTutorial->Draw(x, y - 160.0f, LAYER_TUTORIAL, closeTime / maxCloseTime, 0.0f,
                               {1.0f, 1.0f, 1.0f, closeTime / maxCloseTime});
}

void RadianceTotem::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER)
    {
        close = true;
        if (window->KeyPress(VK_UP))
        {
            TP2::player->State(STILL);
            ready = true;
        }
    }
}
