#ifndef _LEVEL0_H_
#define _LEVEL0_H_

#include "Scene.h"
#include "TP2.h"

class Level0 : public Game
{
  private:
    Sprite *background = nullptr;
    float drawX = 0.0f;
    float drawY = 0.0f;
    static Scene *scene;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();
};

#endif
