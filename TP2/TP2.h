#ifndef _TP2_H_
#define _TP2_H_

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

class TP2 : public Game
{
  private:
    static Level *level;
    HUD *hud;
    Sprite *pauseScreen = nullptr;
    Sprite *transitionScreen = nullptr;

    static bool transitioning;
    static Cooldown levelTransition;

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

    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void StartTransition();

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
