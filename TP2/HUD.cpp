#include "HUD.h"
#include "TP2.h"

HUD::HUD()
{
    bgTileSet = new TileSet("Resources/Hud.png", 1, 2);
    hpTileSet = new TileSet("Resources/Hp.png", 1, 4);
    manaTileSet = new TileSet("Resources/Mp.png", 1, 4);

    bg = new Animation(bgTileSet, 0.0f, false);

    uint seqZero[1] = {0};
    uint seqOne[4] = {1, 2, 1, 3};

    for (int i = 0; i < 5; i++)
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
    delete bgTileSet;
    delete hpTileSet;
    delete manaTileSet;
    delete bg;
    for (int i = 0; i < 5; i++)
        delete hpBar[i];
    for (int i = 0; i < 3; i++)
        delete manaBar[i];
}

void HUD::Update()
{
    if (TP2::player->HasFireball())
    {
        manaBarVisible = true;
        bg->Frame(1);
    }
    else
    {
        manaBarVisible = false;
        bg->Frame(0);
    }

    for (int i = 0; i < 5; i++)
    {
        hpBar[i]->Select(TP2::player->Hp() > i ? 1 : 0);
        hpBar[i]->NextFrame();
    }

    for (int i = 0; i < 3; i++)
    {
        uint frame = max(0, min(3, TP2::player->Mana() - i * 3));
        manaBar[i]->Frame(frame);
    }
}

void HUD::Draw()
{
    bg->Draw(78.0f, 49.0f, LAYER_HUD_BG);

    for (int i = 0; i < 5; i++)
        hpBar[i]->Draw(34.0f + (22.0f * i), 34.0f, LAYER_HUD_FG);

    if (manaBarVisible)
        for (int i = 0; i < 3; i++)
            manaBar[i]->Draw(34.0f + (22.0f * i), 64.0f, LAYER_HUD_FG);
}
