
#include "TP2.h"
#include "Util.h"

Game *TP2::level = nullptr;
Player *TP2::player = nullptr;
Audio *TP2::audio = nullptr;
Scene *TP2::scene = nullptr;
bool TP2::viewBBox = false;
bool TP2::paused = false;

void TP2::Init()
{
    pauseScreen = new Sprite("Resources/PauseScreen.png");

    hud = new HUD();

    audio = new Audio();
    player = new Player();
    level = new Home();
    level->Init();
}

void TP2::Update()
{
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
        pauseScreen->Draw(window->CenterX(), window->CenterY(), 0.0f);

    hud->Draw();
}

void TP2::Finalize()
{
    delete hud;
    delete audio;
    delete player;
    delete level;
    delete pauseScreen;
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
