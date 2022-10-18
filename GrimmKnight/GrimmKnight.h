#ifndef _GRIMMKNIGHT_H_
#define _GRIMMKNIGHT_H_

#include "Audio.h"
#include "Cooldown.h"
#include "Cursor.h"
#include "Engine.h"
#include "Game.h"
#include "HUD.h"
#include "Level.h"
#include "Player.h"
#include "Resources.h"
#include "Scene.h"
#include "TitleScreen.h"

class GrimmKnight : public Game
{
  private:
    static Level *level;
    HUD *hud;
    Sprite *pauseScreen = nullptr;

    bool quit = false;

    Sprite *transitionScreen = nullptr;
    static bool transitioning;
    static Cooldown levelTransition;

    Sprite *fireballScreen = nullptr;
    static bool gettingFireball;
    static Cooldown fireballCd;

    Sprite *dashScreen = nullptr;
    static bool gettingDash;
    static Cooldown dashCd;

  public:
    static LevelId currentLevel;
    static Scene *pausedScene;
    static Player *player;
    static Audio *audio;
    static Scene *scene;
    static Cursor *cursor;
    static bool viewBBox;
    static bool paused;
    static bool baldurKilled;
    static bool fkDefeated;
    static bool playerDead;
    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void StartTransition();
    static void GetFireball();
    static void GetDash();

    template <class T> static void NextLevel()
    {
        if (level != nullptr)
        {
            LevelId id = level->Id();
            level->Finalize();
            delete level;
            scene = nullptr;
            level = new T();
            level->Init();
            level->EnterFrom(id);
            currentLevel = level->Id();
        }
    }
};

#endif
