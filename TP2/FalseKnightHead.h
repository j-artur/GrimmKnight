#ifndef _FALSE_KNIGHT_HEAD_H
#define _FALSE_KNIGHT_HEAD_H

#include "Animation.h"
#include "Entity.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"

class FalseKnightHead : public Object
{
private:
    TileSet* tileSet;
    Animation* animation;

    HDirection direction = H_LEFT;

public:
    FalseKnightHead(TileSet *ts);
    ~FalseKnightHead();

    void TakeDamage();

    void setDirection(HDirection d);

    void Update();
    void Draw();

};

#endif