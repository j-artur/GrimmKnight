#include "HUD.h"
#include "GrimmKnight.h"

HUD::HUD()
{
    bg = new Sprite("Resources/Hud.png");
    hpTileSet = new TileSet("Resources/Hp.png", 1, 4);
    manaTileSet = new TileSet("Resources/Mp.png", 1, 4);

    uint seqZero[1] = {0};
    uint seqOne[4] = {1, 2, 1, 3};

    for (int i = 0; i < 6; i++)
    {
        hpBar[i] = new Animation(hpTileSet, 0.2f, true);

        hpBar[i]->Add(0, seqZero, 1);
        hpBar[i]->Add(1, seqOne, 4);
    }

    for (int i = 0; i < 3; i++)
        manaBar[i] = new Animation(manaTileSet, 0.0f, false);
}

HUD::~HUD()
{
    delete bg;
    delete hpTileSet;
    delete manaTileSet;
    for (int i = 0; i < 6; i++)
        delete hpBar[i];
    for (int i = 0; i < 3; i++)
        delete manaBar[i];
}

void HUD::Update()
{
    for (int i = 0; i < 6; i++)
    {
        hpBar[i]->Select(GrimmKnight::player->Hp() > i ? 1 : 0);
        hpBar[i]->NextFrame();
    }

    for (int i = 0; i < 3; i++)
    {
        uint frame = max(0, min(3, GrimmKnight::player->Mana() - i * 3));
        manaBar[i]->Frame(frame);
    }
}

void HUD::Draw()
{
    bg->Draw(89.0f, 49.0f, LAYER_HUD_BG);

    for (int i = 0; i < 6; i++)
        hpBar[i]->Draw(34.0f + (22.0f * i), 34.0f, LAYER_HUD_FG);

    for (int i = 0; i < 3; i++)
        manaBar[i]->Draw(34.0f + (22.0f * i), 64.0f, LAYER_HUD_FG);
}
