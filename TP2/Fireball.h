#ifndef _FIREBALL_H_
#define _FIREBALL_H_


#include "Types.h"                    
#include "Object.h"                    
#include "Animation.h"   
#include "TileSet.h"
#include "Player.h"


class Fireball : public Object
{
private:
    TileSet* ts;
    Animation* anim;
    Timer fireballTimer;

    float speed;
    float distance;


public:
    Fireball(Player *player,Facing direction);
    ~Fireball();

    void Update();
    void Draw();
};

inline void Fireball::Draw()
{
    anim->Draw(x, y, z);
}

#endif