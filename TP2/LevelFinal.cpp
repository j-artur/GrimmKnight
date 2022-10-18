#include "LevelFinal.h"
#include "EndScreen.h"
#include "TP2.h"
#include "Util.h"
#include "Wall.h"

void LevelFinal::Init()
{
    id = LEVELFINAL;

    background = new Sprite("Resources/RadianceBg.png");
    foreground = new Sprite("Resources/RadianceFg.png");
    radianceScreen = new Sprite("Resources/RadianceScreen.png");
    whiteScreen = new Sprite("Resources/WhiteScreen.png");

    TP2::audio->Play(SRD_THEME, true);

    scene = new Scene();
    TP2::scene = scene;

    scene->Add(TP2::player, MOVING);

    AddWalls(scene, -1, 0, 1, 22);
    AddWalls(scene, 40, 0, 1, 22);
    AddWalls(scene, -0, 22, 40, 2);
    AddWalls(scene, 14, 20, 12, 2);
}

void LevelFinal::Update()
{
    if (radianceScreenCd.Down())
        radianceScreenCd.Add(gameTime);

    else if (!radianceSpawned)
    {
        radianceSpawned = true;

        rd = new Radiance();
        rd->MoveTo(640.0f, 480.0f);
        scene->Add(rd, MOVING);
    }
    else if (ending)
    {
        TP2::audio->Stop(SRD_THEME);
        if (endingCd.Down())
            endingCd.Add(gameTime);
        else
            TP2::NextLevel<EndScreen>();
    }
    else
    {
        if (enteringCd.Down())
            enteringCd.Add(gameTime);
        scene->Update();
        scene->CollisionDetection();

        if (rd != nullptr && !rd->Alive())
        {
            TP2::audio->Play(SRD_FINAL_HIT);
            ending = true;
        }
    }
}

void LevelFinal::Draw()
{
    if (radianceScreenCd.Down())
        radianceScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                             {1.0f, 1.0f, 1.0f, 3.0f - 3.0f * radianceScreenCd.Ratio()});

    float f = 10.0f - 9.0f * enteringCd.Ratio();
    if (enteringCd.Down())
        background->Draw(window->CenterX(), window->CenterY(), LAYER_BG, 1.0f, 0.0f, {f, f, f, 1.0f});
    else
        background->Draw(window->CenterX(), window->CenterY(), LAYER_BG);

    if (ending)
        whiteScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                          {1.0f, 1.0f, 1.0f, endingCd.Ratio()});

    scene->Draw();
    foreground->Draw(window->CenterX(), window->CenterY(), LAYER_FG);
}

void LevelFinal::Finalize()
{
    TP2::audio->Stop(SRD_THEME);
    delete background;
    delete foreground;
    scene->Remove(TP2::player, MOVING);
    delete scene;
}

void LevelFinal::EnterFrom(LevelId id)
{
    TP2::player->MoveTo(640.0f, 610.0f);
    TP2::player->FullHP();
}
