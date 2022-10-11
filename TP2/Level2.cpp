#include "Level2.h"
#include "Level1.h"
#include "Level4.h"
#include "Spike.h"
#include "Wall.h"
#include "EntityBlock.h"
#include "FalseKnight.h"

FalseKnight* Level2::fk = nullptr;

void Level2::Init()
{
    id = LEVEL2;

    background = new Sprite("Resources/Level2Bg.png");
    foreground = new Sprite("Resources/Level2Fg.png");

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    camera = new Camera();
    camera->MoveTo(window->CenterX(), float(window->Height()));
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(VERTICAL, scene);
    screenTransition->MoveTo(1152.0f, 768.0f);
    scene->Add(screenTransition, STATIC);

    level1Transition = new LevelTransition(LEFT);
    level1Transition->MoveTo(0.0f, 672.0f);
    scene->Add(level1Transition, STATIC);

    level4Transition = new LevelTransition(RIGHT);
    level4Transition->MoveTo(1280.0f, 192.0f);
    scene->Add(level4Transition, STATIC);

    AddWalls(scene, 0, 0, 24, 2);
    AddWalls(scene, 24, 0, 16, 4);
    AddWalls(scene, 0, 2, 2, 17);
    AddWalls(scene, 2, 11, 2, 1);
    AddWalls(scene, 5, 14, 2, 1);
    AddWalls(scene, 2, 17, 2, 1);
    AddWalls(scene, 10, 7, 2, 2);
    AddWalls(scene, 8, 9, 4, 2);
    AddWalls(scene, 20, 8, 20, 2);
    AddWalls(scene, 24, 7, 4, 1);
    AddWalls(scene, 32, 7, 2, 1);
    AddWalls(scene, 36, 10, 2, 2);
    AddWalls(scene, 14, 15, 4, 2);
    AddWalls(scene, 22, 15, 4, 2);
    AddWalls(scene, 29, 18, 2, 2);
    AddWalls(scene, 6, 21, 6, 2);
    AddWalls(scene, 18, 21, 8, 2);
    AddWalls(scene, 18, 21, 8, 2);
    AddWalls(scene, 32, 22, 2, 1);
    AddWalls(scene, 32, 25, 2, 1);
    AddWalls(scene, 0, 23, 34, 2);
    AddWalls(scene, 38, 12, 2, 16);
    AddWalls(scene, 0, 25, 2, 18);
    AddWalls(scene, 2, 31, 1, 2);
    AddWalls(scene, 2, 37, 1, 2);
    AddWalls(scene, 5, 28, 33, 2);
    AddWalls(scene, 6, 30, 2, 12);
    AddWalls(scene, 5, 34, 1, 2);
    AddWalls(scene, 5, 40, 1, 2);
    AddWalls(scene, 36, 30, 2, 13);
    AddWalls(scene, 38, 42, 1, 1);
    AddWalls(scene, 2, 43, 1, 3);
    AddWalls(scene, 3, 46, 36, 2);
    AddWalls(scene, 39, 43, 1, 3);

    scene->Add(new EntityBlockBossActivator(10, 30, 16, FALSE_KNIGHT), STATIC);

    fk = new FalseKnight(29, 43);

    scene->Add(fk, MOVING);

    scene->Add(new Spike(26, 4, 6, 1, DOWN), STATIC);
    scene->Add(new Spike(28, 7, 4, 1, UP), STATIC);
}

void Level2::Update()
{
    if (level1Transition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        level1Transition->Update();
    }
    else if (level1Transition->Done())
    {
        TP2::NextLevel<Level1>();
    }
    // else if (level4Transition->Transitioning())
    // {
    //     TP2::player->AddCooldowns(gameTime);
    //     TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    //     TP2::player->UpdateAnimation();
    //     level4Transition->Update();
    // }
    // else if (level4Transition->Done())
    // {
    //     TP2::NextLevel<Level4>();
    // }
    else if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        if (enteringFrom == LEVEL1)
            TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        else if (enteringFrom == LEVEL3)
            TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    }
    else if (screenTransition->Transitioning())
    {
        screenTransition->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
    }
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }
}

void Level2::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void Level2::Finalize()
{
    delete background;
    delete foreground;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level2::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL4:
        enteringFrom = LEVEL4;
        TP2::player->MoveTo(2560.0f, 158.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(H_LEFT);
        enteringCd.Restart();
        break;
    case LEVEL1:
    default:
        enteringFrom = LEVEL1;
        TP2::player->MoveTo(0.0f, 706.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(H_RIGHT);
        enteringCd.Restart();
        break;
    }
}
