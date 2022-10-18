#include "Level4.h"
#include "Level2.h"
#include "LevelFinal.h"
#include "Spike.h"
#include "Totem.h"
#include "Wall.h"

void Level4::Init()
{
    id = LEVEL4;

    TP2::audio->Play(ABYSS_MUSIC, true);

    background = new Sprite("Resources/Level4Bg.png");
    foreground = new Sprite("Resources/Level4Fg.png");
    whiteScreen = new Sprite("Resources/WhiteScreen.png");

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    levelTransition = new LevelTransition(LEFT);
    levelTransition->MoveTo(-32.0f, 640.0f);
    scene->Add(levelTransition, STATIC);

    AddWalls(scene, -1, 0, 1, 18);
    AddWalls(scene, 40, 0, 1, 24);
    AddWalls(scene, -1, 22, 41, 2);

    radianceTotem = new RadianceTotem(24, 22);
    scene->Add(radianceTotem, STATIC);
}

void Level4::Update()
{
    if (radianceTotem->Ready())
    {
        ready = true;
        if (readyCd.Down())
            readyCd.Add(gameTime);
        else
            TP2::NextLevel<LevelFinal>();
    }
    else if (levelTransition->Transitioning())
    {
        TP2::player->AddCooldowns(gameTime);
        TP2::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        TP2::player->UpdateAnimation();
        levelTransition->Update();
    }
    else if (levelTransition->Done())
        TP2::NextLevel<Level2>();
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
    if (ready)
        whiteScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                          {1.0f, 1.0f, 1.0f, readyCd.Ratio()});
    scene->Draw();
}

void Level4::Finalize()
{
    TP2::audio->Stop(ABYSS_MUSIC);
    delete background;
    delete foreground;
    delete whiteScreen;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void Level4::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL2:
        enteringFrom = LEVEL2;
        TP2::player->MoveTo(0.0f, 674.0f);
        TP2::player->State(WALKING);
        TP2::player->Dir(H_RIGHT);
        enteringCd.Restart();
        break;
    case GAMEOVERSCREEN:
    default:
        enteringFrom = GAMEOVERSCREEN;
        TP2::player->MoveTo(112.0f, 674.0f);
        TP2::player->Dir(H_RIGHT);
        break;
    }
}
