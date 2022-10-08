#ifndef _HUD_H_
#define _HUD_H_

#include "Animation.h"

class HUD
{
  private:
    TileSet *bgTileSet;
    TileSet *hpTileSet;
    TileSet *manaTileSet;
    Animation *bg;
    Animation *hpBar[5];
    Animation *manaBar[3];

    bool manaBarVisible = false;

  public:
    HUD();
    ~HUD();

    void Update();
    void Draw();
};

#endif
