#include "TestLevel.h"
#include "Beam.h"
#include "Radiance.h"
#include "Wall.h"

Scene *TestLevel::scene = nullptr;

void TestLevel::Init()
{
    id = LEVELFINAL;

    background = new Sprite("Resources/Level0Bg.png");

    scene = new Scene();
    GrimmKnight::scene = scene;
    GrimmKnight::audio->Stop(MAIN_MUSIC);

    GrimmKnight::player = new Player();
    GrimmKnight::player->MoveTo(256.0f, -32.0f);
    scene->Add(GrimmKnight::player, MOVING);

    Radiance *boss = new Radiance();
    boss->MoveTo(800.0f, 400.0f);
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
    scene->Draw();
}

void TestLevel::Finalize()
{
    delete background;
    scene->Remove(GrimmKnight::player, MOVING);
    delete scene;
}

void TestLevel::EnterFrom(LevelId id)
{
}
