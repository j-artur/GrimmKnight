#ifndef _HOME_H_
#define _HOME_H_

#include "Game.h"
#include "Sprite.h"

class Home : public Game
{
  private:
    Sprite *background = nullptr;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();
};

#endif
