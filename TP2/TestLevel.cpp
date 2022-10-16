#include "TestLevel.h"
#include "Radiance.h"
#include "Beam.h"
#include "Wall.h"

Scene *TestLevel::scene = nullptr;

void TestLevel::Init()
{
    background = new Sprite("Resources/Level0Bg.png");
    // foreground = new Sprite("Resources/Level0Fg.png");

    scene = new Scene();
    TP2::scene = scene;
    TP2::audio->Stop(MAIN_MUSIC);

    TP2::player = new Player();
    TP2::player->MoveTo(256.0f, -32.0f);
    scene->Add(TP2::player, MOVING);

    //boss = new FalseKnight(12, 12);
    Radiance* boss = new Radiance();
    boss->MoveTo(800.0f, 400.0f);
    scene->Add(boss, MOVING);

    Vector v;
    v.ScaleTo(35);
    v.RotateTo(45.0f);
    Beam* beam = new Beam(v);

    //beam->MoveTo(500.0f, 200.0f);
    //scene->Add(beam, STATIC);

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
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void TestLevel::EnterFrom(LevelId id)
{
}

void TestLevel::Respawn()
{
}
