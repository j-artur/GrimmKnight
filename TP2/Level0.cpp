#include "Level0.h"

Scene *Level0::scene = nullptr;

void Level0::Init()
{
    background = new Sprite("Resources/Level0Bg.png");
    foreground = new Sprite("Resources/Level0Fg.png");
    drawX = window->Width();
    drawY = window->CenterY();

    scene = new Scene();
    TP2::scene = scene;

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
    AddWalls(scene, 70, 8, 2, 4);
    AddWalls(scene, 78, 2, 2, 16);
    AddWalls(scene, 66, 12, 12, 2);
}

void Level0::Update()
{
    scene->Update();
    scene->CollisionDetection();
}

void Level0::Draw()
{
    background->Draw(drawX, drawY, Layer::BACK);
    foreground->Draw(drawX, drawY, 0.9f);
    scene->Draw();
}

void Level0::Finalize()
{
    delete background;
    delete scene;
}
