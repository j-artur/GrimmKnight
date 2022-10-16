#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include "Level.h"
#include "Sprite.h"

class GameOver : public Level
{
  private:
    Sprite *bg = nullptr;
    LevelId enterFrom;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
