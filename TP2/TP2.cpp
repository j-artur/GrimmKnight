
#include "TP2.h"
#include "Level0.h"
#include "Level1.h"
#include "Util.h"

#define M_PI 3.141592653589793238462643383279502884L

Level *TP2::level = nullptr;
Player *TP2::player = nullptr;
Audio *TP2::audio = nullptr;
Scene *TP2::scene = nullptr;
bool TP2::viewBBox = false;
bool TP2::paused = false;

bool TP2::transitioning = false;
Cooldown TP2::levelTransition{2.0f};

void TP2::Init()
{
    pauseScreen = new Sprite("Resources/PauseScreen.png");
    transitionScreen = new Sprite("Resources/TransitionScreen.png");

    player = new Player();
    hud = new HUD();
    audio = new Audio();

    level = new Home();
    level->Init();
}

void TP2::Update()
{
    if (window->KeyDown(VK_F1))
        NextLevel<Level0>();
    if (window->KeyDown(VK_F2))
        NextLevel<Level1>();

    if (window->KeyDown('B') && !bBoxKeyCtrl)
    {
        viewBBox = !viewBBox;
        bBoxKeyCtrl = true;
    }
    if (window->KeyUp('B'))
        bBoxKeyCtrl = false;

    if (window->KeyDown(VK_ESCAPE) && !pauseKeyCtrl)
    {
        pauseKeyCtrl = true;
        paused = !paused;
    }
    if (window->KeyUp(VK_ESCAPE))
        pauseKeyCtrl = false;

    if (!paused)
    {
        level->Update();
        hud->Update();
    }
}

void TP2::Draw()
{
    level->Draw();

    if (viewBBox && scene != nullptr)
        scene->DrawBBox();

    if (paused)
        pauseScreen->Draw(window->CenterX(), window->CenterY(), LAYER_PAUSE_SCREEN);

    hud->Draw();

    if (transitioning)
    {
        transitionScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                               {1.0f, 1.0f, 1.0f, (float)sin(M_PI * levelTransition.Time() / 2.0f)});
        levelTransition.Add(gameTime);
        if (levelTransition.Up())
            transitioning = false;
    }
}

void TP2::Finalize()
{
    delete hud;
    delete audio;
    delete player;
    delete level;
    delete pauseScreen;
}

void TP2::StartTransition()
{
    transitioning = true;
    levelTransition.Restart();
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    Engine *engine = new Engine();

    engine->window->Color(0, 0, 0);
    engine->window->Mode(WINDOWED);
    engine->window->Size(SCREEN_WIDTH, SCREEN_HEIGHT);
    engine->window->Title("TP2");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    // engine->window->HideCursor(true);

    int status = engine->Start(new TP2());

    delete engine;
    return status;
}
