#include "GameOver.h"
#include "Button.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TP2.h"
#include "TitleScreen.h"
#include "Util.h"

void GameOver::Init()
{
    id = GAMEOVERSCREEN;

    TP2::audio->Play(GAME_OVER_SONG, true);

    bg = new Sprite("Resources/GameOverScreen.png");
    TP2::playerDead = false;

    scene = new Scene();
    TP2::scene = scene;
    scene->Add(TP2::cursor, MOVING);

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
        TP2::player->Respawn();
        switch (enterFrom)
        {
        case LEVEL0:
            TP2::NextLevel<Level0>();
            break;
        case LEVEL1:
            TP2::NextLevel<Level1>();
            break;
        case LEVEL2:
            TP2::NextLevel<Level2>();
            break;
        case LEVEL3:
            TP2::NextLevel<Level3>();
            break;
        case LEVEL4:
            TP2::NextLevel<Level4>();
            break;
        default:
            TP2::NextLevel<TitleScreen>();
            break;
        }
    }
    else if (quit)
        TP2::NextLevel<TitleScreen>();
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
    scene->Remove(TP2::cursor, MOVING);
    delete scene;
    TP2::audio->Stop(GAME_OVER_SONG);
}

void GameOver::EnterFrom(LevelId id)
{
    enterFrom = id;
}
