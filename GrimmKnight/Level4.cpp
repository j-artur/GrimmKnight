#include "Level4.h"
#include "Level2.h"
#include "LevelFinal.h"
#include "Spike.h"
#include "Totem.h"
#include "Wall.h"

void Level4::Init()
{
    id = LEVEL4;

    GrimmKnight::audio->Play(ABYSS_MUSIC, true);

    background = new Sprite("Resources/Level4Bg.png");
    foreground = new Sprite("Resources/Level4Fg.png");
    whiteScreen = new Sprite("Resources/WhiteScreen.png");

    scene = new Scene();
    GrimmKnight::scene = scene;

    scene->Add(GrimmKnight::player, MOVING);

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
            GrimmKnight::NextLevel<LevelFinal>();
    }
    else if (levelTransition->Transitioning())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->Translate(-LevelTransition::DISTANCE * gameTime, 0.0f);
        GrimmKnight::player->UpdateAnimation();
        levelTransition->Update();
    }
    else if (levelTransition->Done())
        GrimmKnight::NextLevel<Level2>();
    else if (enteringCd.Down())
    {
        GrimmKnight::player->AddCooldowns(gameTime);
        GrimmKnight::player->UpdateAnimation();
        enteringCd.Add(gameTime);
        GrimmKnight::player->Translate(LevelTransition::DISTANCE * gameTime, 0.0f);
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
    GrimmKnight::audio->Stop(ABYSS_MUSIC);
    delete background;
    delete foreground;
    delete whiteScreen;
    scene->Remove(GrimmKnight::player, MOVING);
    delete scene;
}

void Level4::EnterFrom(LevelId id)
{
    switch (id)
    {
    case LEVEL2:
        enteringFrom = LEVEL2;
        GrimmKnight::player->MoveTo(0.0f, 674.0f);
        GrimmKnight::player->State(WALKING);
        GrimmKnight::player->Dir(H_RIGHT);
        enteringCd.Restart();
        break;
    case GAMEOVERSCREEN:
    default:
        enteringFrom = GAMEOVERSCREEN;
        GrimmKnight::player->MoveTo(112.0f, 674.0f);
        GrimmKnight::player->Dir(H_RIGHT);
        break;
    }
}
