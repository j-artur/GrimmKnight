#ifndef _ENDSCREEN_H_
#define _ENDSCREEN_H_

#include "Cooldown.h"
#include "Level.h"
#include "Scene.h"
#include "Sprite.h"

class EndScreen : public Level
{
  private:
    Sprite *bg = nullptr;
    Cooldown cd{4.0f, false};

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
