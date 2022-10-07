#ifndef _FALSE_KNIGHT_H_
#define _FALSE_KNIGHT_H_

#include "Animation.h"
#include "Cooldown.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"

class FalseKnight : public Object
{
private:
    TileSet* tileSet;
    Animation* animation;

    float  ySpeed = 0.0f;
    float jumpingSpeed = -464.0f;
    float gravity = 768.0f;

public:
    

    FalseKnight();
    ~FalseKnight();

    void Update();
    void Draw();
    void OnCollision(Object* other);
};


#endif