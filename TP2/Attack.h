#ifndef _ATTACK_H_
#define _ATTACK_H_


#include "Types.h"                    
#include "Object.h"                    
#include "Animation.h"   
#include "TileSet.h"
#include "Player.h"


class Attack : public Object
{
private:
    TileSet* ts;
    Animation* anim; 
    Player* player;
    AttackDirection direction;


public:
    Attack(Player * player, AttackDirection direction);      
    ~Attack();                   

    void Update();                      
    void Draw();                       
};

inline void Attack::Draw()
{
    anim->Draw(x, y, z);
}

#endif