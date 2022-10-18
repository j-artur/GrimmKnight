#include "TitleScreen.h"
#include "Button.h"
#include "GrimmKnight.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TestLevel.h"
#include "TitleScreen.h"

void TitleScreen::Init()
{
    id = TITLESCREEN;

    GrimmKnight::audio->Play(MAIN_MUSIC, true);

    bgTileSet = new TileSet("Resources/TitleScreen.png", 1282, 770, 3, 6);
    bgAnimation = new Animation(bgTileSet, 0.5f, true);

    scene = new Scene();
    GrimmKnight::scene = scene;
    scene->Add(GrimmKnight::cursor, MOVING);

    GrimmKnight::player->Respawn();
    GrimmKnight::player->UnlearnFireball();
    GrimmKnight::player->UnlearnDash();

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
        GrimmKnight::NextLevel<Level0>();
    }
    else if (window->KeyDown('T'))
        GrimmKnight::NextLevel<TestLevel>();
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
    scene->Remove(GrimmKnight::cursor, MOVING);
    delete scene;
    GrimmKnight::audio->Stop(MAIN_MUSIC);
}

void TitleScreen::EnterFrom(LevelId id)
{
}
