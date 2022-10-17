#include "TitleScreen.h"
#include "Button.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TP2.h"
#include "TestLevel.h"
#include "TitleScreen.h"

void TitleScreen::Init()
{
    id = TITLESCREEN;

    TP2::audio->Play(MAIN_MUSIC, true);

    bgTileSet = new TileSet("Resources/TitleScreen.png", 1282, 770, 3, 6);
    bgAnimation = new Animation(bgTileSet, 0.5f, true);

    scene = new Scene();
    TP2::scene = scene;
    scene->Add(TP2::cursor, MOVING);

    TP2::player->Respawn();
    TP2::player->UnlearnFireball();
    TP2::player->UnlearnDash();

    Button *start = new Button(new Sprite("Resources/ButtonStart.png"), [&]() { this->start = true; });
    start->MoveTo(360.0f, 360.0f);
    scene->Add(start, STATIC);

    Button *quit = new Button(new Sprite("Resources/ButtonQuit.png"), []() { Engine::window->Close(); });
    quit->MoveTo(360.0f, 480.0f);
    scene->Add(quit, STATIC);
}

void TitleScreen::Update()
{
    if (start)
    {
        TP2::NextLevel<Level0>();
    }
    else if (window->KeyDown('T'))
        TP2::NextLevel<TestLevel>();
    else
    {
        scene->Update();
        scene->CollisionDetection();
        bgAnimation->NextFrame();
    }
}

void TitleScreen::Draw()
{
    bgAnimation->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
    scene->Draw();
}

void TitleScreen::Finalize()
{
    delete bgAnimation;
    delete bgTileSet;
    scene->Remove(TP2::cursor, MOVING);
    delete scene;
    TP2::audio->Stop(MAIN_MUSIC);
}

void TitleScreen::EnterFrom(LevelId id)
{
}
