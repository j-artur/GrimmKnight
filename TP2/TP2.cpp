
#include "TP2.h"
#include "Button.h"
#include "GameOver.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "TitleScreen.h"
#include "Util.h"

Level *TP2::level = nullptr;
Player *TP2::player = nullptr;
Audio *TP2::audio = nullptr;
Scene *TP2::scene = nullptr;
Cursor *TP2::cursor = nullptr;
Cooldown TP2::levelTransition{2.0f};
Cooldown TP2::fireballCd{4.0f};
Cooldown TP2::dashCd{4.0f};
LevelId TP2::currentLevel = TITLESCREEN;
Scene *TP2::pausedScene = nullptr;
bool TP2::viewBBox = false;
bool TP2::paused = false;
bool TP2::baldurKilled = false;
bool TP2::fkDefeated = false;
bool TP2::transitioning = false;
bool TP2::gettingFireball = false;
bool TP2::gettingDash = false;
bool TP2::playerDead = false;

void TP2::Init()
{
    pauseScreen = new Sprite("Resources/PauseScreen.png");
    transitionScreen = new Sprite("Resources/TransitionScreen.png");
    fireballScreen = new Sprite("Resources/LearnFireball.png");
    dashScreen = new Sprite("Resources/LearnDash.png");

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
    audio->Add(ENEMY_DREAM_DAMAGE, "Resources/Audio/dream_damage.wav");
    audio->Add(ENEMY_CRAWLER_1, "Resources/Audio/enemy_crawler.wav");
    audio->Add(ENEMY_CRAWLER_2, "Resources/Audio/enemy_crawler.wav");
    audio->Add(ENEMY_CRAWLER_3, "Resources/Audio/enemy_crawler.wav");
    audio->Add(ENEMY_FOOTSTEP_1, "Resources/Audio/husk_footstep.wav");
    audio->Add(ENEMY_FOOTSTEP_2, "Resources/Audio/husk_footstep.wav");
    audio->Add(ENEMY_RUN, "Resources/Audio/husk_run.wav");
    audio->Add(ENEMY_DEATH, "Resources/Audio/husk_death.wav");

    audio->Add(MAIN_MUSIC, "Resources/Audio/main_theme.wav");
    audio->Add(ABYSS_MUSIC, "Resources/Audio/abyss.wav");
    audio->Add(LEVEL3_MUSIC, "Resources/Audio/level_3_theme.wav");
    audio->Add(OPENING, "Resources/Audio/opening.wav");
    audio->Add(CAVE_NOISES, "Resources/Audio/cave_noises.wav");
    audio->Add(CAVE_WIND, "Resources/Audio/cave_wind.wav");

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
    audio->Add(SFK_DEFEAT, "Resources/Audio/FalseKnight/boss_defeat.wav");

    audio->Add(SRD_THEME, "Resources/Audio/Radiance/radiance_theme.wav");
    audio->Add(SRD_FINAL_HIT, "Resources/Audio/Radiance/radiance_final_hit.wav");
    audio->Add(SRD_LASER_BURST, "Resources/Audio/Radiance/radiance_laser_burst.wav");
    audio->Add(SRD_LASER_PREPARE, "Resources/Audio/Radiance/radiance_laser_prepare.wav");
    audio->Add(SRD_ORB, "Resources/Audio/Radiance/radiance_orb.wav");
    audio->Add(SRD_SWORD_BURST, "Resources/Audio/Radiance/radiance_sword_burst.wav");
    audio->Add(SRD_SWORD_WALL, "Resources/Audio/Radiance/radiance_sword_wall.wav");
    audio->Add(SRD_TELEPORT, "Resources/Audio/Radiance/radiance_teleport.wav");

    audio->Add(BALDUR_BLOCK_SOUND, "Resources/Audio/block.wav");

    audio->Add(SOUL_TOTEM, "Resources/Audio/soul_totem_slash.wav");

    audio->Add(GAME_OVER_SONG, "Resources/Audio/game_over.wav");

    pausedScene = new Scene();
    pausedScene->Add(cursor, MOVING);

    Button *resume = new Button(new Sprite("Resources/ButtonResume.png"), [&]() { paused = false; });
    resume->MoveTo(240.0f, 320.0f);
    pausedScene->Add(resume, STATIC);

    Button *quit = new Button(new Sprite("Resources/ButtonQuit.png"), [&]() { this->quit = true; });
    quit->MoveTo(240.0f, 448.0f);
    pausedScene->Add(quit, STATIC);

    level = new TitleScreen();
    level->Init();
    level->EnterFrom(LEVEL0);
}

void TP2::Update()
{
    if (quit)
    {
        paused = false;
        quit = false;
        NextLevel<TitleScreen>();
    }
    else if (playerDead)
        NextLevel<GameOver>();
    else if (window->KeyPress(VK_F1))
        NextLevel<Level0>();
    else if (window->KeyPress(VK_F2))
        NextLevel<Level1>();
    else if (window->KeyPress(VK_F3))
        NextLevel<Level2>();
    else if (window->KeyPress(VK_F4))
        NextLevel<Level3>();
    else if (window->KeyPress(VK_F5))
        NextLevel<Level4>();
    else if (window->KeyPress(VK_F6))
        NextLevel<TitleScreen>();
    else if (window->KeyPress(VK_F7))
        GetFireball();
    else if (window->KeyPress(VK_F8))
        GetDash();
    else if (window->KeyPress('H'))
        player->FullHP();
    else if (window->KeyPress('M'))
        player->FullMana();

    if (window->KeyPress('B'))
        viewBBox = !viewBBox;
    if (window->KeyPress(VK_ESCAPE) && currentLevel != TITLESCREEN && currentLevel != ENDSCREEN)
        paused = !paused;

    if (transitioning)
    {
        levelTransition.Add(gameTime);
        if (levelTransition.Up())
            transitioning = false;
    }

    if (paused)
    {
        pausedScene->Update();
        pausedScene->CollisionDetection();
    }
    else if (gettingFireball)
    {
        if (fireballCd.Up())
        {
            gettingFireball = false;
            player->LearnFireball();
            if (currentLevel == LEVEL2)
                Level2::leftGate->Open();
        }
        else
            fireballCd.Add(gameTime);
    }
    else if (gettingDash)
    {
        if (dashCd.Up())
        {
            gettingDash = false;
            player->LearnDash();
        }
        else
            dashCd.Add(gameTime);
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

    if (gettingFireball)
        fireballScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                             {1.0f, 1.0f, 1.0f,
                              (float)max(sin(3.0f * M_PI * fireballCd.Time() / 4.0f),
                                         ceil(sin(3.0f * M_PI * (fireballCd.Time() - 2.0f / 3.0f) / 8.0f)))});

    if (gettingDash)
        dashScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                         {1.0f, 1.0f, 1.0f,
                          (float)max(sin(3.0f * M_PI * dashCd.Time() / 4.0f),
                                     ceil(sin(3.0f * M_PI * (dashCd.Time() - 2.0f / 3.0f) / 8.0f)))});

    if (transitioning)
        transitionScreen->Draw(window->CenterX(), window->CenterY(), LAYER_TRANSITION_SCREEN, 1.0f, 0.0f,
                               {1.0f, 1.0f, 1.0f, (float)sin(M_PI * levelTransition.Time() / 2.0f)});
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

void TP2::GetFireball()
{
    gettingFireball = true;
    fireballCd.Restart();
}

void TP2::GetDash()
{
    gettingDash = true;
    dashCd.Restart();
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
