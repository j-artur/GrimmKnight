#include "Level1.h"
#include "LevelTransition.h"
#include "TP2.h"

void Level1::Init()
{
    id = LEVEL1;

    background = new Sprite("Resources/Level1Bg.png");
    foreground = new Sprite("Resources/Level1Fg.png");

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    camera = new Camera();
    camera->MoveTo(window->CenterX(), float(window->Height()));
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(VERTICAL, scene);
    screenTransition->MoveTo(384.0f, 768.0f);
    scene->Add(screenTransition, STATIC);
}

void Level1::Update()
{
    if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        if (enteringFrom == LEVEL0 || enteringFrom == LEVEL3)
            TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        else if (enteringFrom == LEVEL2)
            TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
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

void Level1::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
}

void Level1::Finalize()
{
    delete background;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level1::EnterFrom(LevelId id)
{
    enteringFrom = id;

    switch (id)
    {
    case LEVEL0:
        TP2::player->MoveTo(0.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        TP2::player->State(WALKING);
        TP2::player->Dir(RIGHT);
        enteringCd.Restart();
        break;
    case LEVEL2:
        TP2::player->MoveTo(1280.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        TP2::player->State(WALKING);
        TP2::player->Dir(LEFT);
        enteringCd.Restart();
        break;
    case LEVEL3:
        TP2::player->MoveTo(0.0f, 706.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(RIGHT);
        enteringCd.Restart();
        break;
    default:
        break;
    }
}
