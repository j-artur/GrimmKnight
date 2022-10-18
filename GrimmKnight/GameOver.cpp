#include "GameOver.h"
#include "Button.h"
#include "GrimmKnight.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TitleScreen.h"
#include "Util.h"

void GameOver::Init()
{
    id = GAMEOVERSCREEN;

    GrimmKnight::audio->Play(GAME_OVER_SONG, true);

    bg = new Sprite("Resources/GameOverScreen.png");
    GrimmKnight::playerDead = false;

    scene = new Scene();
    GrimmKnight::scene = scene;
    scene->Add(GrimmKnight::cursor, MOVING);

    Button *respawn = new Button(new Sprite("Resources/ButtonRespawn.png"), [&]() { this->respawn = true; });
    respawn->MoveTo(640.0f, 80.0f);
    scene->Add(respawn, STATIC);

    Button *quit = new Button(new Sprite("Resources/ButtonQuit.png"), [&]() { this->quit = true; });
    quit->MoveTo(640.0f, 688.0f);
    scene->Add(quit, STATIC);
}

void GameOver::Update()
{
    if (respawn)
    {
        GrimmKnight::player->Respawn();
        switch (enterFrom)
        {
        case LEVEL0:
            GrimmKnight::NextLevel<Level0>();
            break;
        case LEVEL1:
            GrimmKnight::NextLevel<Level1>();
            break;
        case LEVEL2:
            GrimmKnight::NextLevel<Level2>();
            break;
        case LEVEL3:
            GrimmKnight::NextLevel<Level3>();
            break;
        case LEVEL4:
        case LEVELFINAL:
            GrimmKnight::NextLevel<Level4>();
            break;
        default:
            GrimmKnight::NextLevel<TitleScreen>();
            break;
        }
    }
    else if (quit)
        GrimmKnight::NextLevel<TitleScreen>();
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }
}

void GameOver::Draw()
{
    bg->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
    scene->Draw();
}

void GameOver::Finalize()
{
    delete bg;
    scene->Remove(GrimmKnight::cursor, MOVING);
    delete scene;
    GrimmKnight::audio->Stop(GAME_OVER_SONG);
}

void GameOver::EnterFrom(LevelId id)
{
    enterFrom = id;
}
