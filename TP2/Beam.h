#ifndef _BEAM_H_
#define _BEAM_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Vector.h"

class Beam : public Object
{
  private:
    TileSet *tileSet = nullptr;
    Animation *animation = nullptr;

    Mixed *bb = nullptr;
    Circle *circles[20];
    Vector direction;

    Cooldown beamSpawnCd{0.75f, false};
    Cooldown beamDeactivateCd{1.5f, false};
    Cooldown beamDeleteCd{1.75f, false};

    bool spawnCtrl = true;
    bool deactivateCtrl = true;

  public:
    Beam(TileSet *tileSet, float angle);
    ~Beam();

    void Update();
    void Draw();
};

#endif
