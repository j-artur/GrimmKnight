#ifndef _LEVEL3_H_
#define _LEVEL3_H_

#include "ActionArea.h"
#include "Camera.h"
#include "DashmasterTotem.h"
#include "Level.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"

class Level3 : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    Sprite *totemLeft = nullptr;
    Sprite *totemRight = nullptr;

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition0 = nullptr;
    ScreenTransition *screenTransition1 = nullptr;
    ScreenTransition *screenTransition2 = nullptr;
    ScreenTransition *screenTransition3 = nullptr;
    LevelTransition *levelTransition = nullptr;
    Cooldown enteringCd{LevelTransition::DURATION};

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
