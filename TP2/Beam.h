#ifndef _BEAM_H_
#define _BEAM_H_

#include "Animation.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Cooldown.h"
#include "Vector.h"

class Beam : public Object
{
private:
    TileSet* tileSet;
    Animation* animation;

    Mixed* bb;
    Circle * circles[20];
    Vector direction;

    Cooldown beamSpawnCd{ 0.75f, false};
    Cooldown beamDeleteCd{ 1.5f, false };

    bool spawnCtrl = true;

public:
    Beam(Vector direction);
    ~Beam();

    void Update();
    void Draw();

};

#endif