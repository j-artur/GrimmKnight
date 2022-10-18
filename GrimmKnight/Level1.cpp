#include "Level1.h"
#include "Baldur.h"
#include "EntityBlock.h"
#include "GrimmKnight.h"
#include "Level0.h"
#include "Level2.h"
#include "Level3.h"
#include "LevelTransition.h"
#include "Totem.h"
#include "Wall.h"
#include "WanderingHusk.h"

void Level1::Init()
{
    id = LEVEL1;

    GrimmKnight::audio->Play(CAVE_NOISES, true);

    background = new Sprite("Resources/Level1Bg.png");
    foreground = new Sprite("Resources/Level1Fg.png");
    wanderingTileSet = new TileSet("Resources/WanderingHusk.png", 4, 3);
    totem = new Sprite("Resources/TotemLeft.png");

    scene = new Scene();
    GrimmKnight::scene = scene;

    scene->Add(GrimmKnight::player, MOVING);

    camera = new Camera();
    camera->MoveTo(window->CenterX(), float(window->Height()));
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(VERTICAL, scene);
    screenTransition->MoveTo(384.0f, 768.0f);
    scene->Add(screenTransition, STATIC);

    level0Transition = new LevelTransition(LEFT);
    level0Transition->MoveTo(0.0f, 1408.0f);
    scene->Add(level0Transition, STATIC);

    level2Transition = new LevelTransition(RIGHT);
    level2Transition->MoveTo(1280.0f, 1408.0f);
    scene->Add(level2Transition, STATIC);

    level3Transition = new LevelTransition(LEFT);
    level3Transition->MoveTo(0.0f, 640.0f);
    scene->Add(level3Transition, STATIC);

    AddWalls(scene, 0, 0, 40, 10);
    AddWalls(scene, 0, 10, 2, 8);
    AddWalls(scene, 2, 10, 2, 4);
    AddWalls(scene, 4, 10, 2, 2);
    AddWalls(scene, 18, 10, 2, 2);
    AddWalls(scene, 20, 10, 2, 4);
    AddWalls(scene, 22, 10, 18, 16);
    AddWalls(scene, 14, 22, 8, 4);
    AddWalls(scene, 38, 26, 2, 16);
    AddWalls(scene, 36, 34, 2, 6);
    AddWalls(scene, 34, 36, 2, 4);
    AddWalls(scene, 24, 38, 10, 2);
    AddWalls(scene, 0, 22, 10, 12);
    AddWalls(scene, 10, 28, 2, 6);
    AddWalls(scene, 12, 30, 2, 4);
    AddWalls(scene, 14, 32, 18, 2);
    AddWalls(scene, 0, 34, 2, 8);
    AddWalls(scene, 0, 46, 40, 2);
    AddWalls(scene, 6, 44, 18, 2);
    AddWalls(scene, 8, 42, 14, 2);

    scene->Add(new EntityBlockLeft(7, 38, 5), STATIC);
    scene->Add(new EntityBlockRight(22, 38, 4), STATIC);
    scene->Add(new EntityBlockLeft(13, 26, 6), STATIC);
    scene->Add(new EntityBlockRight(32, 26, 6), STATIC);

    scene->Add(new WanderingHusk(wanderingTileSet, 13, 41, ENEMY_FOOTSTEP_1), MOVING);
    scene->Add(new WanderingHusk(wanderingTileSet, 20, 31, ENEMY_FOOTSTEP_2), MOVING);

    scene->Add(new Totem(totem, 17, 20), STATIC);

    if (!GrimmKnight::baldurKilled)
        scene->Add(new Baldur(3, 20), MOVING);
}

void Level1::Update()
{
    if (level0Transition->Transitioning())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        level0Transition->Update();
    }
    else if (level0Transition->Done())
    {
        GrimmKnight::NextLevel<Level0>();
    }
    else if (level2Transition->Transitioning())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        level2Transition->Update();
    }
    else if (level2Transition->Done())
    {
        GrimmKnight::NextLevel<Level2>();
    }
    else if (level3Transition->Transitioning())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        level3Transition->Update();
    }
    else if (level3Transition->Done())
    {
        GrimmKnight::NextLevel<Level3>();
    }
    else if (enteringCd.Down())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        if (enteringFrom == LEVEL0 || enteringFrom == LEVEL3)
            GrimmKnight::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        else if (enteringFrom == LEVEL2)
            GrimmKnight::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
    }
    else if (screenTransition->Transitioning())
    {
        screenTransition->Update();
        GrimmKnight::player->AddCooldowns(0.1f * gameTime);
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
    delete foreground;
    delete wanderingTileSet;
    delete totem;
    scene->Remove(GrimmKnight::player, MOVING);
    delete scene;
    GrimmKnight::audio->Stop(CAVE_NOISES);
    GrimmKnight::audio->Stop(ENEMY_FOOTSTEP_1);
    GrimmKnight::audio->Stop(ENEMY_FOOTSTEP_2);
}

void Level1::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL2:
        enteringFrom = LEVEL2;
        GrimmKnight::player->MoveTo(1280.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        GrimmKnight::player->State(WALKING);
        GrimmKnight::player->Dir(H_LEFT);
        enteringCd.Restart();
        break;
    case LEVEL3:
        enteringFrom = LEVEL3;
        GrimmKnight::player->MoveTo(0.0f, 674.0f);
        GrimmKnight::player->State(WALKING);
        GrimmKnight::player->Dir(H_RIGHT);
        enteringCd.Restart();
        break;
    case LEVEL0:
        enteringFrom = LEVEL0;
        GrimmKnight::player->MoveTo(0.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        GrimmKnight::player->State(WALKING);
        GrimmKnight::player->Dir(H_RIGHT);
        enteringCd.Restart();
        break;
    case GAMEOVERSCREEN:
    default:
        enteringFrom = GAMEOVERSCREEN;
        GrimmKnight::player->MoveTo(112.0f, 1442.0f);
        scene->Apply([&](Object *obj) { obj->Translate(0.0f, float(-window->Height())); });
        GrimmKnight::player->Dir(H_RIGHT);
        break;
    }
}
