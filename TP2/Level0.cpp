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

    scene->Add(new RightWall(5, 0, 6), STATIC);
    scene->Add(new BottomWall(4, 5, 2), STATIC);
    scene->Add(new RightWall(3, 6, 2), STATIC);
    scene->Add(new BottomWall(2, 7, 2), STATIC);
    scene->Add(new RightWall(1, 8, 10), STATIC);
    scene->Add(new TopWall(2, 18, 2), STATIC);
    scene->Add(new RightWall(3, 18, 2), STATIC);
    scene->Add(new TopWall(4, 20, 8), STATIC);
    scene->Add(new LeftWall(12, 18, 2), STATIC);
    scene->Add(new TopWall(12, 18, 2), STATIC);
    scene->Add(new RightWall(13, 18, 4), STATIC);
    scene->Add(new TopWall(14, 22, 8), STATIC);
    scene->Add(new LeftWall(22, 20, 2), STATIC);
    scene->Add(new TopWall(22, 20, 12), STATIC);
    scene->Add(new LeftWall(34, 18, 2), STATIC);
    scene->Add(new TopWall(34, 18, 4), STATIC);
    scene->Add(new LeftWall(38, 14, 4), STATIC);
    scene->Add(new BottomWall(36, 13, 2), STATIC);
    scene->Add(new LeftWall(36, 12, 2), STATIC);
    scene->Add(new BottomWall(34, 11, 2), STATIC);
    scene->Add(new LeftWall(34, 10, 2), STATIC);
    scene->Add(new BottomWall(28, 9, 6), STATIC);
    scene->Add(new LeftWall(28, 8, 2), STATIC);
    scene->Add(new TopWall(28, 8, 8), STATIC);
    scene->Add(new RightWall(35, 8, 2), STATIC);
    scene->Add(new TopWall(36, 10, 10), STATIC);
    scene->Add(new LeftWall(46, 8, 2), STATIC);
    scene->Add(new TopWall(46, 8, 2), STATIC);
    scene->Add(new RightWall(48, 8, 2), STATIC);
    scene->Add(new TopWall(48, 6, 26), STATIC);
    scene->Add(new RightWall(73, 6, 2), STATIC);
    scene->Add(new BottomWall(50, 7, 2), STATIC);
    scene->Add(new RightWall(49, 8, 2), STATIC);
    scene->Add(new BottomWall(48, 9, 2), STATIC);
    scene->Add(new RightWall(47, 10, 2), STATIC);
    scene->Add(new BottomWall(42, 11, 6), STATIC);
    scene->Add(new RightWall(41, 12, 4), STATIC);
    scene->Add(new TopWall(42, 16, 8), STATIC);
    scene->Add(new RightWall(49, 16, 1), STATIC);
    scene->Add(new TopWall(50, 17, 4), STATIC);
    scene->Add(new LeftWall(54, 16, 1), STATIC);
    scene->Add(new TopWall(54, 16, 10), STATIC);
    scene->Add(new RightWall(54, 16, 2), STATIC);
    scene->Add(new BottomWall(56, 17, 8), STATIC);
    scene->Add(new RightWall(55, 18, 4), STATIC);
    scene->Add(new TopWall(56, 22, 14), STATIC);
    scene->Add(new LeftWall(70, 18, 4), STATIC);
    scene->Add(new TopWall(70, 18, 2), STATIC);
    scene->Add(new RightWall(71, 18, 4), STATIC);
    scene->Add(new TopWall(72, 22, 8), STATIC);
    scene->Add(new BottomWall(80, 24, 2), STATIC);
    scene->Add(new LeftWall(78, 14, 4), STATIC);
    scene->Add(new BottomWall(66, 13, 12), STATIC);
    scene->Add(new LeftWall(66, 12, 2), STATIC);
    scene->Add(new TopWall(66, 12, 12), STATIC);
    scene->Add(new LeftWall(78, 2, 10), STATIC);
    scene->Add(new BottomWall(44, 1, 34), STATIC);
    scene->Add(new RightWall(43, 2, 2), STATIC);
    scene->Add(new BottomWall(42, 3, 2), STATIC);
    scene->Add(new RightWall(41, 4, 2), STATIC);
    scene->Add(new BottomWall(38, 5, 4), STATIC);
    scene->Add(new LeftWall(38, 4, 2), STATIC);
    scene->Add(new BottomWall(36, 3, 2), STATIC);
    scene->Add(new LeftWall(36, 2, 2), STATIC);
    scene->Add(new BottomWall(24, 1, 12), STATIC);
    scene->Add(new RightWall(23, 2, 2), STATIC);
    scene->Add(new BottomWall(22, 3, 2), STATIC);
    scene->Add(new RightWall(21, 4, 6), STATIC);
    scene->Add(new TopWall(22, 10, 2), STATIC);
    scene->Add(new RightWall(23, 10, 4), STATIC);
    scene->Add(new TopWall(24, 14, 8), STATIC);
    scene->Add(new RightWall(31, 14, 2), STATIC);
    scene->Add(new BottomWall(22, 15, 10), STATIC);
    scene->Add(new LeftWall(22, 12, 4), STATIC);
    scene->Add(new BottomWall(20, 11, 2), STATIC);
    scene->Add(new LeftWall(20, 10, 2), STATIC);
    scene->Add(new BottomWall(18, 9, 2), STATIC);
    scene->Add(new LeftWall(18, 8, 2), STATIC);
    scene->Add(new BottomWall(12, 7, 6), STATIC);
    scene->Add(new LeftWall(12, 6, 2), STATIC);
    scene->Add(new BottomWall(10, 5, 2), STATIC);
    scene->Add(new LeftWall(10, 0, 6), STATIC);
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
