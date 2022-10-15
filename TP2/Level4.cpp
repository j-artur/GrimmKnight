#include "Level4.h"
#include "Level2.h"
#include "Spike.h"
#include "Totem.h"
#include "Wall.h"

void Level4::Init()
{
    id = LEVEL4;

    background = new Sprite("Resources/Level4Bg.png");
    foreground = new Sprite("Resources/Level4Fg.png");

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    levelTransition = new LevelTransition(LEFT);
    levelTransition->MoveTo(0.0f, 640.0f);
    scene->Add(levelTransition, STATIC);

    AddWalls(scene, 1, 0, 1, 18);
    AddWalls(scene, 40, 0, 1, 24);
    AddWalls(scene, 0, 22, 40, 2);


    // scene->Add(new EntityBlockLeft(7, 30, 16), STATIC);
    // scene->Add(new EntityBlockTop(8, 29, 28), STATIC);
    // scene->Add(new EntityBlockRight(36, 30, 16), STATIC);
    // scene->Add(new EntityBlockBottom(8, 46, 28), STATIC);

}

void Level4::Update()
{

    if (levelTransition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        levelTransition->Update();
    }
    else if (levelTransition->Done())
    {
        TP2::NextLevel<Level2>();
    }
    else if (enteringCd.Down())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        TP2::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
    }
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }
}

void Level4::Draw()
{
    background->Draw(window->CenterX(), window->CenterY(), LAYER_BG);
    foreground->Draw(window->CenterX(), window->CenterY(), LAYER_FG);
    scene->Draw();
}

void Level4::Finalize()
{
    delete background;
    delete foreground;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}
void Level4::EnterFrom(LevelId id)
{
    enteringFrom = LEVEL1;
    TP2::player->MoveTo(0.0f, 674.0f);
    TP2::player->State(WALKING);
    TP2::player->Dir(H_RIGHT);
    enteringCd.Restart();
}
