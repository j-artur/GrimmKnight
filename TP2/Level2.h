#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include "ActionArea.h"
#include "Camera.h"
#include "EntityBlock.h"
#include "FalseKnight.h"
#include "Gate.h"
#include "Level.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"

class Level2 : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    TileSet *tiktikTileSet = nullptr;
    TileSet *wanderingTileSet = nullptr;
    Image *gate = nullptr;

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;
    LevelTransition *level1Transition = nullptr;
    LevelTransition *level4Transition = nullptr;

    LevelId enteringFrom;
    Cooldown enteringCd{LevelTransition::DURATION};

    ActionArea *fireballArea = nullptr;

  public:
    static FalseKnight *fk;
    static Gate *leftGate;
    static Gate *rightGate;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
