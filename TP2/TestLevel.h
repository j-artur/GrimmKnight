#ifndef _TESTLEVEL_H_
#define _TESTLEVEL_H_

#include "FalseKnight.h"
#include "Level.h"
#include "Scene.h"
#include "TP2.h"

class TestLevel : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;

    FalseKnight *boss;

    static Scene *scene;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
    void Respawn();
};

#endif
