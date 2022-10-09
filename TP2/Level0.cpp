#include "Level0.h"
#include "EntityBlock.h"
#include "Level1.h"
#include "Spike.h"
#include "Tiktik.h"
#include "Wall.h"

void Level0::Init()
{
    id = LEVEL0;

    background = new Sprite("Resources/Level0Bg.png");
    foreground = new Sprite("Resources/Level0Fg.png");
    tiktikTileSet = new TileSet("Resources/Tiktik.png", 2, 4);

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    camera = new Camera();
    camera->MoveTo((float)window->Width(), window->CenterY());
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(HORIZONTAL, scene);
    screenTransition->MoveTo((float)window->Width(), 256.0f);
    scene->Add(screenTransition, STATIC);

    level1Transition = new LevelTransition(ATK_RIGHT);
    level1Transition->MoveTo(2560.0f, 640.0f);
    scene->Add(level1Transition, STATIC);

    AddWalls(scene, 6, 15, 4, 1);
    AddWalls(scene, 12, 8, 8, 1);
    AddWalls(scene, 22, 13, 2, 3);
    AddWalls(scene, 30, 15, 3, 1);
    AddWalls(scene, 16, 15, 2, 1);
    AddWalls(scene, 18, 15, 1, 4);
    AddWalls(scene, 19, 18, 1, 1);
    AddWalls(scene, 26, 9, 2, 1);
    AddWalls(scene, 28, 7, 1, 3);
    AddWalls(scene, 4, 0, 2, 6);
    AddWalls(scene, 2, 6, 2, 2);
    AddWalls(scene, 0, 8, 2, 10);
    AddWalls(scene, 2, 18, 2, 2);
    AddWalls(scene, 4, 20, 8, 4);
    AddWalls(scene, 12, 18, 2, 4);
    AddWalls(scene, 14, 22, 8, 2);
    AddWalls(scene, 22, 20, 12, 2);
    AddWalls(scene, 26, 14, 2, 6);
    AddWalls(scene, 34, 18, 4, 2);
    AddWalls(scene, 38, 14, 2, 4);
    AddWalls(scene, 38, 10, 10, 2);
    AddWalls(scene, 36, 10, 2, 4);
    AddWalls(scene, 38, 4, 4, 2);
    AddWalls(scene, 36, 2, 8, 2);
    AddWalls(scene, 10, 0, 70, 2);
    AddWalls(scene, 40, 12, 2, 4);
    AddWalls(scene, 46, 8, 28, 2);
    AddWalls(scene, 42, 16, 8, 1);
    AddWalls(scene, 50, 17, 4, 1);
    AddWalls(scene, 54, 16, 12, 2);
    AddWalls(scene, 54, 18, 2, 4);
    AddWalls(scene, 56, 22, 24, 2);
    AddWalls(scene, 70, 18, 2, 4);
    AddWalls(scene, 78, 2, 2, 16);
    AddWalls(scene, 70, 12, 8, 2);

    scene->Add(new Spike(14, 21, 8, 1, ATK_UP), STATIC);
    scene->Add(new Spike(28, 19, 6, 1, ATK_UP), STATIC);
    scene->Add(new Spike(50, 16, 4, 1, ATK_UP), STATIC);
    scene->Add(new Spike(50, 10, 4, 1, ATK_DOWN), STATIC);

    scene->Add(new EntityBlockLeft(11, 2, 6), STATIC);
    scene->Add(new EntityBlockRight(20, 2, 6), STATIC);
    scene->Add(new EntityBlockLeft(45, 2, 6), STATIC);
    scene->Add(new EntityBlockRight(74, 2, 6), STATIC);

    scene->Add(new Tiktik(tiktikTileSet, 16, 7), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 55, 7), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 67, 7), MOVING);
}

void Level0::Update()
{
    if (level1Transition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        level1Transition->Update();
    }
    else if (level1Transition->Done())
    {
        TP2::NextLevel<Level1>();
    }
    else if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
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

void Level0::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void Level0::Finalize()
{
    delete background;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level0::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL1:
        scene->Apply([&](Object *obj) { obj->Translate(float(-window->Width()), 0.0f); });
        TP2::player->MoveTo(1280.0f, 674.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(LEFT);
        enteringCd.Restart();
        break;
    case TITLESCREEN:
    default:
        TP2::player->MoveTo(256.0f, -34.0f);
        break;
    }
}
