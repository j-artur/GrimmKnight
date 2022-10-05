#ifndef _TP2_H_
#define _TP2_H_

#include "Audio.h"
#include "Engine.h"
#include "Game.h"
#include "Home.h"
#include "Player.h"
#include "Resources.h"
#include "Scene.h"

class TP2 : public Game
{
  private:
    static Game *level;
    Sprite *pauseScreen = nullptr;
    bool pauseKeyCtrl = false;
    bool bBoxKeyCtrl = false;

  public:
    static Player *player;
    static Audio *audio;
    static Scene *scene;
    static bool viewBBox;
    static bool paused;
    static Sprite *solidTile;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    template <class T> static void NextLevel()
    {
        if (level != nullptr)
        {
            level->Finalize();
            delete level;
            scene = nullptr;
            level = new T();
            level->Init();
        }
    }
};

#endif
