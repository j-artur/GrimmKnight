#include "EndScreen.h"
#include "Button.h"
#include "TP2.h"
#include "TitleScreen.h"

void EndScreen::Init()
{
    id = ENDSCREEN;

    TP2::audio->Play(MAIN_MUSIC, true);

    bgTileSet = new TileSet("Resources/TitleScreen.png", 1282, 770, 3, 6);
    bgAnimation = new Animation(bgTileSet, 0.5f, true);

    scene = new Scene();
    TP2::scene = scene;
    scene->Add(TP2::cursor, MOVING);

    Button *quit = new Button(new Sprite("Resources/ButtonQuit.png"), []() { Engine::window->Close(); });
    quit->MoveTo(360.0f, 480.0f);
    scene->Add(quit, STATIC);
}

void EndScreen::Update()
{
    scene->Update();
    scene->CollisionDetection();
    bgAnimation->NextFrame();
}

void EndScreen::Draw()
{
    bgAnimation->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
    scene->Draw();
}

void EndScreen::Finalize()
{
    delete bgAnimation;
    delete bgTileSet;
    scene->Remove(TP2::cursor, MOVING);
    delete scene;
    TP2::audio->Stop(MAIN_MUSIC);
}

void EndScreen::EnterFrom(LevelId id)
{
}
