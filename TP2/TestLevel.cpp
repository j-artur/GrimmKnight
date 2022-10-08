#include "TestLevel.h"
#include "Wall.h"

Scene *TestLevel::scene = nullptr;

void TestLevel::Init()
{
    background = new Sprite("Resources/Level0Bg.png");
    // foreground = new Sprite("Resources/Level0Fg.png");

    scene = new Scene();
    TP2::scene = scene;

    TP2::player = new Player();
    TP2::player->MoveTo(256.0f, -32.0f);
    scene->Add(TP2::player, MOVING);

    boss = new FalseKnight();
    boss->MoveTo(400.0f, -100.0f);
    scene->Add(boss, MOVING);

    AddWalls(scene, 0, 22, 40, 2);
}

void TestLevel::Update()
{

    scene->Update();
    scene->CollisionDetection();
}

void TestLevel::Draw()
{
    // background->Draw(camera->X(), camera->Y(), LAYER_BG);
    // foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void TestLevel::Finalize()
{
    delete background;
    delete scene;
}
