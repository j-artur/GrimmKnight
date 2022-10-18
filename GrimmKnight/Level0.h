#ifndef _LEVEL0_H_
#define _LEVEL0_H_

#include "Camera.h"
#include "GrimmKnight.h"
#include "Level.h"
#include "LevelTransition.h"
#include "Scene.h"
#include "ScreenTransition.h"

enum Tutorial
{
    TUTORIAL_BEGIN,
    TUTORIAL_MOVE_0,
    TUTORIAL_MOVE_1,
    TUTORIAL_MOVE_2,
    TUTORIAL_JUMP_0,
    TUTORIAL_JUMP_1,
    TUTORIAL_JUMP_2,
    TUTORIAL_ATTACK_0,
    TUTORIAL_ATTACK_1,
    TUTORIAL_ATTACK_2,
    TUTORIAL_HEAL_0,
    TUTORIAL_HEAL_1,
    TUTORIAL_HEAL_2,
    TUTORIAL_OVER,
};

class Level0 : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    Sprite *totemSprite = nullptr;
    TileSet *tiktikTileSet = nullptr;
    TileSet *wanderingTileSet = nullptr;
    Sprite *tutorialMove = nullptr;
    Sprite *tutorialJump = nullptr;
    Sprite *tutorialAttack = nullptr;
    Sprite *tutorialHeal = nullptr;

    Scene *scene = nullptr;

    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;
    LevelTransition *level1Transition = nullptr;

    Cooldown enteringCd{LevelTransition::DURATION};

    Tutorial tutorial = TUTORIAL_BEGIN;
    Cooldown tutorialTransitionCd{0.25f, false};
    Cooldown tutorialCd{1.25f, false};

  public:
    static int tikTikCounter;

    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
