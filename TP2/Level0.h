#ifndef _LEVEL0_H_
#define _LEVEL0_H_

#include "Camera.h"
#include "Level.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"

class Level0 : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    TileSet *tiktikTileSet = nullptr;
    TileSet* wanderingTileSet = nullptr;

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;
    LevelTransition *level1Transition = nullptr;

    Cooldown enteringCd{LevelTransition::DURATION};


  public:
    static int tikTikCounter;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
