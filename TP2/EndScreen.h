#ifndef _ENDSCREEN_H_
#define _ENDSCREEN_H_

#include "Animation.h"
#include "Level.h"
#include "Scene.h"

class EndScreen : public Level
{
  private:
    TileSet *bgTileSet = nullptr;
    Animation *bgAnimation = nullptr;
    Scene *scene = nullptr;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
