#include "Attack.h"
#include "TP2.h"

Attack::Attack(Player * player, AttackDirection direction)
{
    ts = new TileSet("Resources/attack.png", 64, 64, 5, 10);
    anim = new Animation(ts, 0.1f, false);
    this->player = player;
    this->direction = direction;

    int width = ts->TileWidth();
    int height = ts->TileHeight();

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
}

// ---------------------------------------------------------------------------------

Attack::~Attack()
{
    delete ts;
    delete anim;
}

// ---------------------------------------------------------------------------------

void Attack::Update()
{
    switch (direction){
    case UP:
        MoveTo(player->X(), player->Y() - 50.0f);
        break;
    case DOWN:
        MoveTo(player->X(), player->Y() + 50.0f);
        break;
    case LEFT:
        MoveTo(player->X() - 50.0f, player->Y() );
        break;
    case RIGHT:
        MoveTo(player->X() + 50.0f, player->Y());
        break;
    default:
        break;
    }
    
    anim->NextFrame();
    if (anim->Inactive())
        TP2::scene->Delete();
}