#include "GrimmTotem.h"
#include "GrimmKnight.h"
#include "Player.h"
#include "Util.h"

GrimmTotem::GrimmTotem(int iX, int iY)
{
    tileSet = new TileSet("Resources/Grimm.png", 1, 2);
    anim = new Animation(tileSet, 0.0f, false);
    interactTutorial = new Sprite("Resources/TutorialInteract.png");

    uint seqEnabled[1] = {0};
    uint seqDisabled[1] = {1};

    anim->Add(0, seqEnabled, 1);
    anim->Add(1, seqDisabled, 1);

    anim->Select(GrimmKnight::player->HasFireball());

    BBox(new Rect(-32.0f, 50.0f, 32.0f, 50.0f));

    MoveTo(iX * 32.0f + 26.0f, iY * 32.0f - 50.0f);
}

GrimmTotem::~GrimmTotem()
{
    delete anim;
    delete tileSet;
    delete interactTutorial;
}

void GrimmTotem::Update()
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

    anim->NextFrame();
}

void GrimmTotem::Draw()
{
    anim->Draw(X(), Y(), LAYER_TOTEM);
    if (!GrimmKnight::player->HasFireball() && closeTime > 0.0f)
        interactTutorial->Draw(x - 120.0f, window->CenterY() + 160.0f, LAYER_TUTORIAL, closeTime / maxCloseTime, 0.0f,
                               {1.0f, 1.0f, 1.0f, closeTime / maxCloseTime});
}

void GrimmTotem::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER)
    {
        close = true;
        if (!GrimmKnight::player->HasFireball() && window->KeyPress(VK_UP))
        {
            GrimmKnight::player->State(STILL);
            GrimmKnight::GetFireball();
            anim->Select(1);
        }
    }
}
