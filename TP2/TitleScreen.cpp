#include "TitleScreen.h"
#include "Level0.h"
#include "TP2.h"
#include "TestLevel.h"

void TitleScreen::Init()
{
    id = TITLESCREEN;

    bgTileSet = new TileSet("Resources/TitleScreen.png", 1282, 770, 3, 6);
    bgAnimation = new Animation(bgTileSet, 0.5f, true);
}

void TitleScreen::Update()
{
    if (window->KeyDown(VK_SPACE))
        TP2::NextLevel<Level0>();

    else if (window->KeyDown('T'))
        TP2::NextLevel<TestLevel>();
    else
        bgAnimation->NextFrame();
}

void TitleScreen::Draw()
{
    bgAnimation->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
}

void TitleScreen::Finalize()
{
    delete bgAnimation;
    delete bgTileSet;
}

void TitleScreen::EnterFrom(LevelId id)
{
}
