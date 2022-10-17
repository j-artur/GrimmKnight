#ifndef _LEVELFINAL_H_
#define _LEVELFINAL_H_

#include "Cooldown.h"
#include "Level.h"
#include "Radiance.h"
#include "Scene.h"
#include "Sprite.h"

class LevelFinal : public Level
{
  private:
    Sprite *background = nullptr;
    Sprite *foreground = nullptr;
    Sprite *radianceScreen = nullptr;
    Sprite *whiteScreen = nullptr;

    Scene *scene = nullptr;
    Cooldown enteringCd{1.0f, false};

    Radiance *rd = nullptr;
    Cooldown radianceScreenCd{2.0f, false};
    bool radianceSpawned = false;

    bool ending = false;
    Cooldown endingCd{2.0f, false};

  public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    void EnterFrom(LevelId id);
};

#endif
