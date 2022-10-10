#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include "Animation.h"
#include "Level.h"

class TitleScreen : public Level
{
  private:
    TileSet *bgTileSet = nullptr;
    Animation *bgAnimation = nullptr;

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
