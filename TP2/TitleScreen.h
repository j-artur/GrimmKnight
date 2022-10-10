#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include "Animation.h"
#include "Level.h"
#include "Scene.h"

class TitleScreen : public Level
{
  private:
    TileSet *bgTileSet = nullptr;
    Animation *bgAnimation = nullptr;
    Scene *scene = nullptr;

    bool start = false;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
