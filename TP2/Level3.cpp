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
    AddWalls(scene, 68, 44, 2, 2);
    AddWalls(scene, 72, 41, 2, 2);
    AddWalls(scene, 76, 38, 2, 2);
    AddWalls(scene, 72, 35, 2, 2);
    AddWalls(scene, 51, 44, 3, 2);
    AddWalls(scene, 48, 41, 3, 3);
    AddWalls(scene, 45, 38, 3, 3);
    AddWalls(scene, 42, 35, 3, 3);
    AddWalls(scene, 35, 32, 7, 3);
    AddWalls(scene, 38, 35, 2, 11);
    AddWalls(scene, 11, 46, 27, 2);
    AddWalls(scene, 4, 44, 7, 2);
    AddWalls(scene, 2, 42, 2, 2);
    AddWalls(scene, 2, 36, 2, 2);
    AddWalls(scene, 2, 32, 4, 4);
    AddWalls(scene, 2, 22, 22, 4);
    AddWalls(scene, 10, 20, 2, 2);
    AddWalls(scene, 8, 18, 2, 2);
    AddWalls(scene, 6, 16, 2, 2);
    AddWalls(scene, 4, 14, 2, 2);
    AddWalls(scene, 2, 12, 2, 2);
    AddWalls(scene, 7, 2, 2, 4);
    AddWalls(scene, 7, 10, 2, 2);
    AddWalls(scene, 12, 12, 2, 2);
    AddWalls(scene, 18, 10, 2, 2);
    AddWalls(scene, 24, 8, 2, 2);
    AddWalls(scene, 30, 6, 4, 2);
    AddWalls(scene, 32, 8, 2, 14);
    AddWalls(scene, 34, 18, 12, 2);
    AddWalls(scene, 46, 15, 2, 3);
    AddWalls(scene, 48, 10, 2, 5);
    AddWalls(scene, 46, 8, 25, 2);
    AddWalls(scene, 42, 11, 2, 2);
    AddWalls(scene, 38, 2, 4, 12);
    AddWalls(scene, 63, 6, 4, 2);
    AddWalls(scene, 67, 7, 4, 1);
    AddWalls(scene, 71, 6, 2, 4);
    AddWalls(scene, 65, 2, 2, 2);
    AddWalls(scene, 67, 2, 4, 1);
    AddWalls(scene, 71, 2, 7, 2);
    AddWalls(scene, 74, 14, 4, 4);
    AddWalls(scene, 70, 18, 4, 4);
    AddWalls(scene, 58, 22, 20, 6);
    AddWalls(scene, 58, 28, 4, 2);
    AddWalls(scene, 62, 28, 4, 1);
    AddWalls(scene, 66, 28, 2, 2);
    AddWalls(scene, 54, 10, 4, 4);
    AddWalls(scene, 52, 14, 2, 22);
    AddWalls(scene, 54, 32, 8, 4);
    AddWalls(scene, 62, 33, 4, 3);
    AddWalls(scene, 66, 32, 4, 4);
    AddWalls(scene, 49, 30, 3, 3);
    AddWalls(scene, 46, 27, 3, 3);
    AddWalls(scene, 42, 26, 4, 1);
    AddWalls(scene, 28, 22, 14, 6);
    AddWalls(scene, 26, 28, 4, 10);
    AddWalls(scene, 30, 38, 3, 1);
    AddWalls(scene, 11, 39, 22, 2);
    AddWalls(scene, 6, 39, 5, 1);
    AddWalls(scene, 21, 36, 1, 3);
    AddWalls(scene, 22, 32, 1, 4);
    AddWalls(scene, 21, 30, 5, 2);
    AddWalls(scene, 9, 33, 2, 2);
    AddWalls(scene, 12, 30, 2, 2);
    AddWalls(scene, 16, 30, 2, 3);
    AddWalls(scene, 16, 33, 3, 3);

    scene->Add(new Spike(54, 45, 4, 1, UP), STATIC);
    scene->Add(new Spike(55, 36, 11, 1, DOWN), STATIC);
    scene->Add(new Spike(51, 41, 1, 3, RIGHT), STATIC);
    scene->Add(new Spike(48, 38, 1, 3, RIGHT), STATIC);
    scene->Add(new Spike(45, 35, 1, 3, RIGHT), STATIC);
    scene->Add(new Spike(42, 32, 1, 3, RIGHT), STATIC);
    scene->Add(new Spike(51, 33, 1, 3, LEFT), STATIC);
    scene->Add(new Spike(48, 30, 1, 3, LEFT), STATIC);
    scene->Add(new Spike(42, 27, 4, 1, DOWN), STATIC);
    scene->Add(new Spike(30, 28, 1, 10, RIGHT), STATIC);
    scene->Add(new Spike(37, 35, 1, 11, LEFT), STATIC);
    scene->Add(new Spike(33, 38, 1, 3, RIGHT), STATIC);
    scene->Add(new Spike(30, 45, 4, 1, UP), STATIC);
    scene->Add(new Spike(25, 41, 4, 1, DOWN), STATIC);
    scene->Add(new Spike(20, 45, 4, 1, UP), STATIC);
    scene->Add(new Spike(15, 41, 4, 1, DOWN), STATIC);
    scene->Add(new Spike(6, 40, 5, 1, DOWN), STATIC);
    scene->Add(new Spike(2, 38, 1, 4, RIGHT), STATIC);
    scene->Add(new Spike(9, 35, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(12, 32, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(2, 2, 5, 1, DOWN), STATIC);
    scene->Add(new Spike(7, 6, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(9, 2, 17, 1, DOWN), STATIC);
    scene->Add(new Spike(11, 12, 1, 2, LEFT), STATIC);
    scene->Add(new Spike(12, 14, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(14, 12, 1, 2, RIGHT), STATIC);
    scene->Add(new Spike(17, 10, 1, 2, LEFT), STATIC);
    scene->Add(new Spike(18, 12, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(20, 10, 1, 2, RIGHT), STATIC);
    scene->Add(new Spike(23, 8, 1, 2, LEFT), STATIC);
    scene->Add(new Spike(24, 10, 2, 1, DOWN), STATIC);
    scene->Add(new Spike(26, 8, 1, 2, RIGHT), STATIC);
    scene->Add(new Spike(37, 2, 1, 12, LEFT), STATIC);
    scene->Add(new Spike(34, 6, 1, 12, RIGHT), STATIC);
    scene->Add(new Spike(67, 3, 4, 1, DOWN), STATIC);
    scene->Add(new Spike(67, 6, 4, 1, UP), STATIC);
    scene->Add(new Spike(62, 29, 4, 1, DOWN), STATIC);
    scene->Add(new Spike(62, 32, 4, 1, UP), STATIC);
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
