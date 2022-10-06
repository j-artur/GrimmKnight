#include "Fireball.h"
#include "TP2.h"
#include "Util.h"

Fireball::Fireball(Player *player, Direction direction)
{
    type = FIREBALL;

    ts = new TileSet("Resources/attack.png", 64, 64, 5, 10);
    anim = new Animation(ts, 0.1f, true);

    int width = 32;
    int height = 32;

    speed = 800.0f;
    distance = 14.0f;

    if (direction == LEFT)
    {
        speed = -speed;
        distance = -distance;
    }

    MoveTo(player->X() + distance, player->Y());

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
}

// ---------------------------------------------------------------------------------

Fireball::~Fireball()
{
    delete ts;
    delete anim;
}

// ---------------------------------------------------------------------------------

void Fireball::Update()
{
    Translate(speed * gameTime, 0);

    if (fireballCd.Ready())
        TP2::scene->Delete();

    fireballCd.Add(gameTime);

    anim->NextFrame();
}

// ---------------------------------------------------------------------------------

void Fireball::OnCollision(Object *obj)
{
    if (obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT)
    {
        TP2::scene->Delete(this, MOVING);
        // anim exploding fireball
    }
}
