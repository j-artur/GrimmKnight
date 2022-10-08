#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "Animation.h"
#include "Object.h"
#include "TileSet.h"
#include "Types.h"
#include "Player.h"

class Shockwave : public Object
{
private:
    TileSet* ts;
    Animation* anim;

    float speed = 800.0f;

public:
    Shockwave(Direction direction, TileSet* tileset);
    ~Shockwave();

    void Update();
    void Draw();
    void OnCollision(Object* other);
};

inline void Shockwave::Draw()
{
    anim->Draw(x, y, z);
}

#endif
