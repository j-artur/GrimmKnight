#ifndef _LEVEL0_H_
#define _LEVEL0_H_

#include "Camera.h"
#include "Scene.h"
#include "ScreenTransition.h"
#include "TP2.h"

class Level0 : public Game
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    Camera *camera = nullptr;
    ScreenTransition *screenTransition = nullptr;
    TileSet *tiktikTileSet = nullptr;

    static Scene *scene;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();
};

#endif
