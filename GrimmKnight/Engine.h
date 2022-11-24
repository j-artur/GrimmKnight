#ifndef _PROGJOGOS_ENGINE_H_
#define _PROGJOGOS_ENGINE_H_

#include "Game.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Timer.h"
#include "Window.h"

class Engine
{
  private:
    static Timer timer;
    static bool paused;

    float FrameTime();
    int Loop();

  public:
    static Game *game;
    static Window *window;
    static Graphics *graphics;
    static Renderer *renderer;
    static float frameTime;

    Engine();
    ~Engine();

    int Start(Game *level);

    static void Pause();
    static void Resume();

    template <class T> static void Next()
    {
        if (game)
        {
            game->Finalize();
            delete game;
            game = new T();
            game->Init();
        }
    };
};

#endif
