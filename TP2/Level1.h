#ifndef _LEVEL1_H_
#define _LEVEL1_H_

#include "Camera.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"

class Level1 : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;

    Cooldown enteringCd{LevelTransition::DURATION};
    LevelId enteringFrom;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
