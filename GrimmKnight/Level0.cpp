#include "Level0.h"
#include "EntityBlock.h"
#include "Level1.h"
#include "Spike.h"
#include "Tiktik.h"
#include "Totem.h"
#include "Wall.h"
#include "WanderingHusk.h"

void Level0::Init()
{
    id = LEVEL0;

    GrimmKnight::audio->Play(CAVE_NOISES, true);

    background = new Sprite("Resources/Level0Bg.png");
    foreground = new Sprite("Resources/Level0Fg.png");
    tiktikTileSet = new TileSet("Resources/Tiktik.png", 2, 4);
    wanderingTileSet = new TileSet("Resources/WanderingHusk.png", 4, 3);
    totemSprite = new Sprite("Resources/TotemRight.png");

    tutorialMove = new Sprite("Resources/TutorialMove.png");
    tutorialJump = new Sprite("Resources/TutorialJump.png");
    tutorialAttack = new Sprite("Resources/TutorialAttack.png");
    tutorialHeal = new Sprite("Resources/TutorialHeal.png");

    scene = new Scene();
    GrimmKnight::scene = scene;

    scene->Add(GrimmKnight::player, MOVING);

    camera = new Camera();
    camera->MoveTo((float)window->Width(), window->CenterY());
    scene->Add(camera, STATIC);

    screenTransition = new ScreenTransition(HORIZONTAL, scene);
    screenTransition->MoveTo((float)window->Width(), 256.0f);
    scene->Add(screenTransition, STATIC);

    level1Transition = new LevelTransition(RIGHT);
    level1Transition->MoveTo(2560.0f, 640.0f);
    scene->Add(level1Transition, STATIC);

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
    AddWalls(scene, 46, 8, 28, 2);
    AddWalls(scene, 42, 16, 8, 1);
    AddWalls(scene, 50, 17, 4, 1);
    AddWalls(scene, 54, 16, 12, 2);
    AddWalls(scene, 54, 18, 2, 4);
    AddWalls(scene, 56, 22, 24, 2);
    AddWalls(scene, 70, 18, 2, 4);
    AddWalls(scene, 78, 2, 2, 16);
    AddWalls(scene, 70, 12, 8, 2);

    scene->Add(new Spike(14, 21, 8, 1, UP), STATIC);
    scene->Add(new Spike(28, 19, 6, 1, UP), STATIC);
    scene->Add(new Spike(50, 16, 4, 1, UP), STATIC);
    scene->Add(new Spike(50, 10, 4, 1, DOWN), STATIC);

    scene->Add(new EntityBlockLeft(11, 2, 6), STATIC);
    scene->Add(new EntityBlockRight(20, 2, 6), STATIC);
    scene->Add(new EntityBlockLeft(45, 2, 6), STATIC);
    scene->Add(new EntityBlockRight(74, 2, 6), STATIC);

    scene->Add(new Tiktik(tiktikTileSet, 16, 7, ENEMY_CRAWLER_1), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 55, 7, ENEMY_CRAWLER_2), MOVING);
    scene->Add(new Tiktik(tiktikTileSet, 67, 7, ENEMY_CRAWLER_3), MOVING);

    scene->Add(new WanderingHusk(wanderingTileSet, 56, 21, ENEMY_FOOTSTEP_1), MOVING);

    scene->Add(new Totem(totemSprite, 44, 14), STATIC);
}

void Level0::Update()
{
    switch (tutorial)
    {
    case TUTORIAL_BEGIN:
        tutorialCd.Add(gameTime);
        if (tutorialCd.Up())
        {
            tutorial = TUTORIAL_MOVE_0;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_MOVE_0:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_MOVE_1;
            tutorialCd.Restart();
        }
        break;
    case TUTORIAL_MOVE_1:
        tutorialCd.Add(gameTime);
        if (tutorialCd.Up())
        {
            tutorial = TUTORIAL_MOVE_2;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_MOVE_2:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_JUMP_0;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_JUMP_0:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_JUMP_1;
            tutorialCd.Restart();
        }
        break;
    case TUTORIAL_JUMP_1:
        tutorialCd.Add(gameTime);
        if (tutorialCd.Up())
        {
            tutorial = TUTORIAL_JUMP_2;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_JUMP_2:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_ATTACK_0;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_ATTACK_0:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_ATTACK_1;
            tutorialCd.Restart();
        }
        break;
    case TUTORIAL_ATTACK_1:
        tutorialCd.Add(gameTime);
        if (tutorialCd.Up())
        {
            tutorial = TUTORIAL_ATTACK_2;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_ATTACK_2:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_HEAL_0;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_HEAL_0:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_HEAL_1;
            tutorialCd.Restart();
        }
        break;
    case TUTORIAL_HEAL_1:
        tutorialCd.Add(gameTime);
        if (tutorialCd.Up())
        {
            tutorial = TUTORIAL_HEAL_2;
            tutorialTransitionCd.Restart();
        }
        break;
    case TUTORIAL_HEAL_2:
        tutorialTransitionCd.Add(gameTime);
        if (tutorialTransitionCd.Up())
        {
            tutorial = TUTORIAL_OVER;
        }
        break;
    }

    if (level1Transition->Transitioning())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        level1Transition->Update();
    }
    else if (level1Transition->Done())
    {
        GrimmKnight::NextLevel<Level1>();
    }
    else if (enteringCd.Down())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        enteringCd.Add(gameTime);
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

void Level0::Draw()
{
    background->Draw(camera->X(), camera->Y(), LAYER_BG);
    foreground->Draw(camera->X(), camera->Y(), LAYER_FG);
    scene->Draw();
    switch (tutorial)
    {
    case TUTORIAL_MOVE_0:
        tutorialMove->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               tutorialTransitionCd.Ratio(),
                           });
        break;
    case TUTORIAL_MOVE_1:
        tutorialMove->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL);

        break;
    case TUTORIAL_MOVE_2:
        tutorialMove->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               1.0f - tutorialTransitionCd.Ratio(),
                           });
        break;
    case TUTORIAL_JUMP_0:
        tutorialJump->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               tutorialTransitionCd.Ratio(),
                           });
        break;
    case TUTORIAL_JUMP_1:
        tutorialJump->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL);
        break;
    case TUTORIAL_JUMP_2:
        tutorialJump->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               1.0f - tutorialTransitionCd.Ratio(),
                           });
        break;
    case TUTORIAL_ATTACK_0:
        tutorialAttack->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                             {
                                 1.0f,
                                 1.0f,
                                 1.0f,
                                 tutorialTransitionCd.Ratio(),
                             });
        break;
    case TUTORIAL_ATTACK_1:
        tutorialAttack->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL);
        break;
    case TUTORIAL_ATTACK_2:
        tutorialAttack->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                             {
                                 1.0f,
                                 1.0f,
                                 1.0f,
                                 1.0f - tutorialTransitionCd.Ratio(),
                             });
        break;
    case TUTORIAL_HEAL_0:
        tutorialHeal->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               tutorialTransitionCd.Ratio(),
                           });
        break;
    case TUTORIAL_HEAL_1:
        tutorialHeal->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL);
        break;
    case TUTORIAL_HEAL_2:
        tutorialHeal->Draw(window->CenterX(), window->CenterY(), LAYER_TUTORIAL, 1.0f, 0.0f,
                           {
                               1.0f,
                               1.0f,
                               1.0f,
                               1.0f - tutorialTransitionCd.Ratio(),
                           });
        break;
    default:
        break;
    }
}

void Level0::Finalize()
{
    delete background;
    delete foreground;
    delete tiktikTileSet;
    delete wanderingTileSet;
    delete tutorialMove;
    delete tutorialJump;
    delete tutorialHeal;
    delete tutorialAttack;
    scene->Remove(GrimmKnight::player, MOVING);
    delete scene;
    GrimmKnight::audio->Stop(CAVE_NOISES);
    GrimmKnight::audio->Stop(ENEMY_CRAWLER_1);
    GrimmKnight::audio->Stop(ENEMY_CRAWLER_2);
    GrimmKnight::audio->Stop(ENEMY_CRAWLER_3);
    GrimmKnight::audio->Stop(ENEMY_FOOTSTEP_1);
}

void Level0::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL1:
        scene->Apply([&](Object *obj) { obj->Translate(float(-window->Width()), 0.0f); });
        GrimmKnight::player->MoveTo(1280.0f, 674.0f);
        GrimmKnight::player->State(WALKING);
        GrimmKnight::player->Dir(H_LEFT);
        enteringCd.Restart();
        tutorial = TUTORIAL_OVER;
        break;
    case TITLESCREEN:
        GrimmKnight::audio->Play(OPENING);

        GrimmKnight::player->MoveTo(256.0f, -34.0f);
        GrimmKnight::player->State(FALLING);
        tutorial = TUTORIAL_BEGIN;
        tutorialCd.Restart();
        tutorialTransitionCd.Restart();
        break;
    case GAMEOVERSCREEN:
    default:
        GrimmKnight::player->MoveTo(256.0f, 450.0f);
        GrimmKnight::player->Dir(H_RIGHT);
        tutorial = TUTORIAL_OVER;
        break;
    }
}
