#include "Level1.h"
#include "Level0.h"
#include "EntityBlock.h"
#include "LevelTransition.h"
#include "TP2.h"
#include "Wall.h"
#include "WanderingHusk.h"
// #include "Level2.h"
// #include "Level3.h"

void Level1::Init()
{
    id = LEVEL1;

    background = new Sprite("Resources/Level1Bg.png");
    foreground = new Sprite("Resources/Level1Fg.png");
    wanderingTileSet = new TileSet("Resources/WIP/WanderingHusk.png", 4, 7);

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    camera = new Camera();
    camera->MoveTo(window->CenterX(), float(window->Height()));
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(VERTICAL, scene);
    screenTransition->MoveTo(384.0f, 768.0f);
    scene->Add(screenTransition, STATIC);

    level0Transition = new LevelTransition(ATK_LEFT);
    level0Transition->MoveTo(0.0f, 1408.0f);
    scene->Add(level0Transition, STATIC);

    level2Transition = new LevelTransition(ATK_RIGHT);
    level2Transition->MoveTo(2560.0f, 1408.0f);
    scene->Add(level2Transition, STATIC);

    level3Transition = new LevelTransition(ATK_LEFT);
    level3Transition->MoveTo(0.0f, 672.0f);
    scene->Add(level3Transition, STATIC);

    AddWalls(scene, 0, 0, 40, 10);
    AddWalls(scene, 0, 10, 2, 10);
    AddWalls(scene, 2, 10, 2, 4);
    AddWalls(scene, 4, 10, 2, 2);
    AddWalls(scene, 18, 10, 2, 2);
    AddWalls(scene, 20, 10, 2, 4);
    AddWalls(scene, 22, 10, 18, 16);
    AddWalls(scene, 14, 22, 8, 4);
    AddWalls(scene, 38, 26, 2, 16);
    AddWalls(scene, 36, 34, 2, 6);
    AddWalls(scene, 34, 36, 2, 4);
    AddWalls(scene, 24, 38, 10, 2);
    AddWalls(scene, 0, 22, 10, 12);
    AddWalls(scene, 10, 28, 2, 6);
    AddWalls(scene, 12, 30, 2, 4);
    AddWalls(scene, 14, 32, 18, 2);
    AddWalls(scene, 0, 34, 2, 8);
    AddWalls(scene, 0, 46, 40, 2);
    AddWalls(scene, 6, 44, 18, 2);
    AddWalls(scene, 8, 42, 14, 2);

    scene->Add(new EntityBlockLeft(7, 38, 5), STATIC);
    scene->Add(new EntityBlockRight(22, 38, 4), STATIC);
    scene->Add(new EntityBlockLeft(13, 26, 6), STATIC);
    scene->Add(new EntityBlockRight(32, 26, 6), STATIC);

    scene->Add(new WanderingHusk(wanderingTileSet, 13, 41), MOVING);
    scene->Add(new WanderingHusk(wanderingTileSet, 16, 28), MOVING);
}

void Level1::Update()
{
    if (level0Transition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        level0Transition->Update();
    }
    else if (level0Transition->Done())
    {
        TP2::NextLevel<Level0>();
    }
    // else if (level2Transition->Transitioning())
    // {
    //     TP2::player->AddCooldowns(gameTime);
    //     TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
    //     TP2::player->UpdateAnimation();
    //     level2Transition->Update();
    // }
    // else if (level2Transition->Done())
    // {
    //     TP2::NextLevel<Level2>();
    // }
    // else if (level3Transition->Transitioning())
    // {
    //     TP2::player->AddCooldowns(gameTime);
    //     TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    //     TP2::player->UpdateAnimation();
    //     level3Transition->Update();
    // }
    // else if (level3Transition->Done())
    // {
    //     TP2::NextLevel<Level3>();
    // }
    else if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        if (enteringFrom == LEVEL0 || enteringFrom == LEVEL3)
            TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        else if (enteringFrom == LEVEL2)
            TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    }
    else if (screenTransition->Transitioning())
    {
        screenTransition->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
        scene->CollisionDetection();
    }
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }
}

void Level1::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void Level1::Finalize()
{
    delete background;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level1::EnterFrom(LevelId id)
{
    enteringFrom = id;

    switch (id)
    {
    case LEVEL2:
        TP2::player->MoveTo(1280.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        TP2::player->State(WALKING);
        TP2::player->Dir(LEFT);
        enteringCd.Restart();
        break;
    case LEVEL3:
        TP2::player->MoveTo(0.0f, 706.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(RIGHT);
        enteringCd.Restart();
        break;
    case LEVEL0:
    default:
        TP2::player->MoveTo(0.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        TP2::player->State(WALKING);
        TP2::player->Dir(RIGHT);
        enteringCd.Restart();
        break;
    }
}
