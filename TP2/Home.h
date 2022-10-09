#ifndef _HOME_H_
#define _HOME_H_

#include "Level.h"
#include "Sprite.h"

class Home : public Level
{
  private:
    Sprite *background = nullptr;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
