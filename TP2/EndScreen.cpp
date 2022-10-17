#include "EndScreen.h"
#include "TP2.h"
#include "TitleScreen.h"
#include "Util.h"

void EndScreen::Init()
{
    id = ENDSCREEN;

    bg = new Sprite("Resources/EndScreen.png");
}

void EndScreen::Update()
{
    if (cd.Up())
        TP2::NextLevel<TitleScreen>();
    else
        cd.Add(gameTime);
}

void EndScreen::Draw()
{
    bg->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
}

void EndScreen::Finalize()
{
    delete bg;
}

void EndScreen::EnterFrom(LevelId id)
{
}
