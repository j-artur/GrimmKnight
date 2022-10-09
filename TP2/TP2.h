#ifndef _TP2_H_
#define _TP2_H_

#include "Audio.h"
#include "Cooldown.h"
#include "Engine.h"
#include "Game.h"
#include "HUD.h"
#include "Home.h"
#include "Level.h"
#include "Player.h"
#include "Resources.h"
#include "Scene.h"

class TP2 : public Game
{
  private:
    static Level *level;
    HUD *hud;
    Sprite *pauseScreen = nullptr;
    Sprite *transitionScreen = nullptr;
    bool pauseKeyCtrl = false;
    bool bBoxKeyCtrl = false;

    static bool transitioning;
    static Cooldown levelTransition;

  public:
    static Player *player;
    static Audio *audio;
    static Scene *scene;
    static bool viewBBox;
    static bool paused;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void StartTransition();

    template <class T> static void NextLevel()
    {
        if (level != nullptr)
        {
            LevelId number = level->Id();
            level->Finalize();
            delete level;
            scene = nullptr;
            level = new T();
            level->Init();
            level->EnterFrom(number);
        }
    }
};

#endif
