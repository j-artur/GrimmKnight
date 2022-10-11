#include "Level3.h"
#include "Level1.h"
#include "Spike.h"
#include "Wall.h"

void Level3::Init()
{
    id = LEVEL3;

    background = new Sprite("Resources/Level3Bg.png");
    foreground = new Sprite("Resources/Level3Fg.png");

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    camera = new Camera();
    camera->MoveTo(float(window->Width()), float(window->Height()));
    scene->Add(camera, STATIC);

    screenTransition0 = new ScreenTransition(HORIZONTAL, scene);
    screenTransition0->MoveTo(1280.0f, 960.0f);
    scene->Add(screenTransition0, STATIC);

    screenTransition1 = new ScreenTransition(VERTICAL, scene);
    screenTransition1->MoveTo(832.0f, 768.0f);
    scene->Add(screenTransition1, STATIC);

    screenTransition2 = new ScreenTransition(HORIZONTAL, scene);
    screenTransition2->MoveTo(1280.0f, 512.0f);
    scene->Add(screenTransition2, STATIC);

    screenTransition3 = new ScreenTransition(VERTICAL, scene);
    screenTransition3->MoveTo(1792.0f, 768.0f);
    scene->Add(screenTransition3, STATIC);

    levelTransition = new LevelTransition(RIGHT);
    levelTransition->MoveTo(2560.0f, 1408.0f);
    scene->Add(levelTransition, STATIC);

    AddWalls(scene, 0, 0, 80, 2);
    AddWalls(scene, 0, 46, 80, 2);
    AddWalls(scene, 0, 0, 2, 48);
    AddWalls(scene, 78, 0, 2, 42);
}

void Level3::Update()
{
    if (levelTransition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        levelTransition->Update();
    }
    else if (levelTransition->Done())
    {
        TP2::NextLevel<Level1>();
    }
    else if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    }
    else if (screenTransition0->Transitioning())
    {
        screenTransition0->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
    }
    else if (screenTransition1->Transitioning())
    {
        screenTransition1->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
    }
    else if (screenTransition2->Transitioning())
    {
        screenTransition2->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
    }
    else if (screenTransition3->Transitioning())
    {
        screenTransition3->Update();
        TP2::player->AddCooldowns(0.1f * gameTime);
    }
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }
}

void Level3::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void Level3::Finalize()
{
    delete background;
    delete foreground;
    delete totem;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level3::EnterFrom(LevelId id)
{
    TP2::player->MoveTo(2560.0f, 1442.0f);
    scene->Apply([&](Object *obj) { obj->Translate(float(-window->Width()), float(-window->Height())); });
    TP2::player->State(WALKING);
    TP2::player->Dir(H_LEFT);
    enteringCd.Restart();
}
