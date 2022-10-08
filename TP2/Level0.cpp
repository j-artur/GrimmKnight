#include "Level0.h"
#include "EntityBlock.h"
#include "Tiktik.h"
#include "Wall.h"

Scene *Level0::scene = nullptr;

void Level0::Init()
{
    background = new Sprite("Resources/Level0Bg.png");
    foreground = new Sprite("Resources/Level0Fg.png");
    tiktikTileSet = new TileSet("Resources/Tiktik.png", 2, 4);

    scene = new Scene();
    TP2::scene = scene;

    camera = new Camera();
    camera->MoveTo((float)window->Width(), window->CenterY());
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(HORIZONTAL, scene);
    screenTransition->MoveTo((float)window->Width(), 256.0f);
    scene->Add(screenTransition, STATIC);

    TP2::player = new Player();
    TP2::player->MoveTo(256.0f, -32.0f);
    scene->Add(TP2::player, MOVING);

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
    AddWalls(scene, 46, 8, 4, 2);
    AddWalls(scene, 48, 6, 26, 2);
    AddWalls(scene, 42, 16, 8, 1);
    AddWalls(scene, 50, 17, 4, 1);
    AddWalls(scene, 54, 16, 10, 2);
    AddWalls(scene, 54, 18, 2, 4);
    AddWalls(scene, 56, 22, 24, 2);
    AddWalls(scene, 70, 18, 2, 4);
    AddWalls(scene, 78, 2, 2, 16);
    AddWalls(scene, 66, 12, 12, 2);

    scene->Add(new EntityBlockRight(11, 2, 6), STATIC);
    scene->Add(new EntityBlockLeft(20, 2, 6), STATIC);
    scene->Add(new EntityBlockRight(47, 2, 4), STATIC);
    scene->Add(new EntityBlockLeft(74, 2, 4), STATIC);

    scene->Add(new Tiktik(tiktikTileSet, 16, 7), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 55, 5), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 67, 5), MOVING);
}

void Level0::Update()
{
    if (screenTransition->Transitioning())
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

void Level0::Draw()
{
    background->Draw(camera->X(), camera->Y(), Layer::BACK);
    foreground->Draw(camera->X(), camera->Y(), 0.9f);
    scene->Draw();
}

void Level0::Finalize()
{
    delete background;
    delete scene;
}
