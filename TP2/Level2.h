#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include "Camera.h"
#include "Level.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"
#include "EntityBlock.h"
#include "FalseKnight.h"

class Level2 : public Level
{
  private:
    RightWall *bossWall;
    bool insertedWall = false;
    bool removedWall = false;

    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    TileSet *tiktikTileSet = nullptr;
    TileSet *wanderingTileSet = nullptr;    

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;
    LevelTransition *level1Transition = nullptr;
    LevelTransition *level4Transition = nullptr;

    LevelId enteringFrom;
    Cooldown enteringCd{LevelTransition::DURATION};

  public:
    static FalseKnight* fk;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
