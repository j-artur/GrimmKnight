#include "GameOver.h"
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
    bg = new Sprite("Resources/GameOverScreen.png");
    TP2::playerDead = false;
}

void GameOver::Update()
{
    if (window->KeyPress(VK_RETURN))
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
}

void GameOver::Draw()
{
    bg->Draw(window->CenterX(), window->CenterY(), LAYER_TITLESCREEN);
}

void GameOver::Finalize()
{
    delete bg;
}

void GameOver::EnterFrom(LevelId id)
{
    enterFrom = id;

    switch (id)
    {
    case LEVEL0:
        OutputDebugString("GameOver::EnterFrom(LEVEL0)\n");
        break;
    case LEVEL1:
        OutputDebugString("GameOver::EnterFrom(LEVEL1)\n");
        break;
    case LEVEL2:
        OutputDebugString("GameOver::EnterFrom(LEVEL2)\n");
        break;
    case LEVEL3:
        OutputDebugString("GameOver::EnterFrom(LEVEL3)\n");
        break;
    case LEVEL4:
        OutputDebugString("GameOver::EnterFrom(LEVEL4)\n");
        break;
    case TITLESCREEN:
        OutputDebugString("GameOver::EnterFrom(TITLESCREEN)\n");
        break;
    case GAMEOVERSCREEN:
        OutputDebugString("GameOver::EnterFrom(GAMEOVER)\n");
        break;
    case ENDSCREEN:
        OutputDebugString("GameOver::EnterFrom(ENDSCREEN)\n");
        break;
    default:
        OutputDebugString("GameOver::EnterFrom(UNKNOWN)\n");
        break;
    }
}
