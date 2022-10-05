#include "Level0.h"

Scene *Level0::scene = nullptr;

void Level0::Init()
{
    background = new Sprite("Resources/Level0.png");
    drawX = window->Width();
    drawY = window->CenterY();

    scene = new Scene();
    TP2::scene = scene;

    TP2::player = new Player();
    TP2::player->MoveTo(256.0f, -32.0f);

    scene->Add(TP2::player, MOVING);

    scene->Add(new Wall(0, 0, 6, 6), STATIC);
    scene->Add(new Wall(0, 6, 4, 2), STATIC);
    scene->Add(new Wall(0, 8, 2, 16), STATIC);
    scene->Add(new Wall(2, 18, 2, 6), STATIC);
    scene->Add(new Wall(4, 20, 8, 4), STATIC);
    scene->Add(new Wall(12, 18, 2, 6), STATIC);
    scene->Add(new Wall(14, 22, 8, 2), STATIC);
    scene->Add(new Wall(22, 20, 12, 4), STATIC);
    scene->Add(new Wall(34, 18, 22, 6), STATIC);
    scene->Add(new Wall(24, 14, 8, 2), STATIC);
    scene->Add(new Wall(22, 10, 2, 6), STATIC);
    scene->Add(new Wall(20, 0, 2, 12), STATIC);
    scene->Add(new Wall(18, 0, 2, 10), STATIC);
    scene->Add(new Wall(12, 0, 6, 8), STATIC);
    scene->Add(new Wall(10, 0, 2, 6), STATIC);
    scene->Add(new Wall(22, 0, 2, 4), STATIC);
    scene->Add(new Wall(24, 0, 56, 2), STATIC);
    scene->Add(new Wall(36, 2, 8, 2), STATIC);
    scene->Add(new Wall(38, 4, 4, 2), STATIC);
    scene->Add(new Wall(28, 8, 8, 2), STATIC);
    scene->Add(new Wall(34, 10, 14, 2), STATIC);
    scene->Add(new Wall(36, 12, 2, 2), STATIC);
    scene->Add(new Wall(38, 12, 4, 6), STATIC);
}

void Level0::Update()
{
    scene->Update();
    scene->CollisionDetection();
}

void Level0::Draw()
{
    background->Draw(drawX, drawY, Layer::BACK);
    scene->Draw();
}

void Level0::Finalize()
{
    delete background;
    delete scene;
}
