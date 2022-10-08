#ifndef _HUD_H_
#define _HUD_H_

#include "Animation.h"

class HUD
{
  private:
    Sprite *bg;
    TileSet *hpTileSet;
    TileSet *manaTileSet;
    Animation *hpBar[5];
    Animation *manaBar[3];

  public:
    HUD();
    ~HUD();

    void Update();
    void Draw();
};

#endif
