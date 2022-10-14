
#include "TP2.h"
#include "Button.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Util.h"

#define M_PI 3.141592653589793238462643383279502884L

Level *TP2::level = nullptr;
Player *TP2::player = nullptr;
Audio *TP2::audio = nullptr;
Scene *TP2::scene = nullptr;
Cursor *TP2::cursor = nullptr;
bool TP2::viewBBox = false;
bool TP2::paused = false;

bool TP2::transitioning = false;
Cooldown TP2::levelTransition{2.0f};
LevelId TP2::currentLevel = TITLESCREEN;
Scene *TP2::pausedScene = nullptr;

void TP2::Init()
{
    pauseScreen = new Sprite("Resources/PauseScreen.png");
    transitionScreen = new Sprite("Resources/TransitionScreen.png");

    cursor = new Cursor();
    player = new Player();
    hud = new HUD();
    audio = new Audio();

    audio->Add(PLAYER_ATTACK, "Resources/Audio/Player/hero_attack.wav");
    audio->Add(PLAYER_SPIKE_ATTACK, "Resources/Audio/Player/hero_spike_attack.wav");
    audio->Add(PLAYER_JUMP, "Resources/Audio/Player/hero_jump.wav");
    audio->Add(PLAYER_LAND, "Resources/Audio/Player/hero_land.wav");
    audio->Add(PLAYER_DASH, "Resources/Audio/Player/hero_dash.wav");
    audio->Add(PLAYER_FIREBALL, "Resources/Audio/Player/hero_fireball.wav");
    audio->Add(PLAYER_HURT, "Resources/Audio/Player/hero_damage.wav");
    audio->Add(PLAYER_WALK, "Resources/Audio/Player/hero_walk_footsteps_stone.wav");

    audio->Add(ENEMY_DAMAGE, "Resources/Audio/enemy_damage.wav");
    audio->Add(ENEMY_CRAWLER, "Resources/Audio/enemy_crawler.wav", 3);

    audio->Add(CAVE_NOISES, "Resources/Audio/cave_noises.wav");
    audio->Add(MAIN_MUSIC, "Resources/Audio/S23-11.wav");

    audio->Add(SFK_THEME, "Resources/Audio/FalseKnight/false_knight_theme.wav");
    audio->Add(SFK_STUN, "Resources/Audio/FalseKnight/false_knight_stun.wav");
    audio->Add(SFK_ARMOR_DAMAGE, "Resources/Audio/FalseKnight/false_knight_damage_armour.wav");
    audio->Add(SFK_HEAD_DAMAGE, "Resources/Audio/FalseKnight/false_knight_head_damage.wav");
    audio->Add(SFK_JUMP, "Resources/Audio/FalseKnight/false_knight_jump.wav");
    audio->Add(SFK_LAND, "Resources/Audio/FalseKnight/false_knight_land.wav");
    audio->Add(SFK_SWING, "Resources/Audio/FalseKnight/false_knight_swing.wav");
    audio->Add(SFK_STRIKE_GROUND, "Resources/Audio/FalseKnight/false_knight_strike_ground.wav");
    audio->Add(SFK_VOICE_ATTACK, "Resources/Audio/FalseKnight/false_knight_attack.wav");
    audio->Add(SFK_VOICE_RAGE, "Resources/Audio/FalseKnight/false_knight_rage.wav");
    audio->Add(SFK_BARREL_DEATH, "Resources/Audio/FalseKnight/barrel_death.wav");
    pausedScene = new Scene();
    pausedScene->Add(cursor, MOVING);

    Button *resume = new Button(new Sprite("Resources/ButtonResume.png"), [&]() { paused = false; });
    resume->MoveTo(640.0f, 96.0f);
    pausedScene->Add(resume, STATIC);

    Button *quit = new Button(new Sprite("Resources/ButtonQuit.png"), []() { Engine::window->Close(); });
    quit->MoveTo(640.0f, 672.0f);
    pausedScene->Add(quit, STATIC);

    level = new TitleScreen();
    level->Init();
}

void TP2::Update()
{
    if (window->KeyDown(VK_F1))
        NextLevel<Level0>();
    else if (window->KeyDown(VK_F2))
        NextLevel<Level1>();
    else if (window->KeyDown(VK_F3))
        NextLevel<Level2>();
    else if (window->KeyDown(VK_F4))
        NextLevel<Level3>();
    else if (window->KeyDown(VK_F6))
        NextLevel<TitleScreen>();
    else if (window->KeyDown(VK_F7))
        player->LearnFireball();
    else if (window->KeyDown(VK_F8))
        player->LearnDash();
    else if (window->KeyDown('H'))
        player->FullHP();
    else if (window->KeyDown('M'))
        player->FullMana();

    if (window->KeyPress('B'))
        viewBBox = !viewBBox;
    if (window->KeyPress(VK_ESCAPE) && currentLevel != TITLESCREEN && currentLevel != ENDSCREEN)
        paused = !paused;

    if (paused)
    {
        pausedScene->Update();
        pausedScene->CollisionDetection();
    }
    else
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

    hud->Draw();

    if (paused)
    {
        pauseScreen->Draw(window->CenterX(), window->CenterY(), LAYER_PAUSE_SCREEN);
        pausedScene->Draw();
    }

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
    engine->window->HideCursor(true);

    int status = engine->Start(new TP2());

    delete engine;
    return status;
}
