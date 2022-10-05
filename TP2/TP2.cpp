
#include "TP2.h"

Game *TP2::level = nullptr;
Player *TP2::player = nullptr;
Audio *TP2::audio = nullptr;
Scene *TP2::scene = nullptr;
bool TP2::viewBBox = false;
bool TP2::paused = false;
Sprite *TP2::solidTile = nullptr;

void TP2::Init()
{
    pauseScreen = new Sprite("Resources/PauseScreen.png");
    solidTile = new Sprite("Resources/SolidTile.png");

    audio = new Audio();
    player = new Player();
    level = new Home();
    level->Init();
}

void TP2::Update()
{
    if (window->KeyDown('B'))
    {
        if (!bBoxKeyCtrl)
        {
            viewBBox = !viewBBox;
            bBoxKeyCtrl = true;
        }
    }
    else
    {
        bBoxKeyCtrl = false;
    }

    if (window->KeyDown(VK_ESCAPE))
    {
        if (!pauseKeyCtrl)
        {
            pauseKeyCtrl = true;
            paused = !paused;
        }
    }
    else
    {
        pauseKeyCtrl = false;
    }

    if (!paused)
        level->Update();
}

void TP2::Draw()
{
    level->Draw();

    if (viewBBox && scene != nullptr)
        scene->DrawBBox();

    if (paused)
    {
        pauseScreen->Draw(window->CenterX(), window->CenterY(), 0.0f);
    }
}

void TP2::Finalize()
{
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
    engine->window->Size(1280, 768);
    engine->window->Title("TP2");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    // engine->window->HideCursor(true);

    int status = engine->Start(new TP2());

    delete engine;
    return status;
}
