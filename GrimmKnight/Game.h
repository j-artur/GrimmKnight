#ifndef _PROGJOGOS_GAME_H_
#define _PROGJOGOS_GAME_H_

#include "Window.h"

class Game
{
  protected:
    static Window *&window;
    static float &gameTime;

  public:
    Game();
    virtual ~Game();

    virtual void OnPause();

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
};

#endif
