#include "Shockwave.h"
#include "TP2.h"
#include "Util.h"

Shockwave::Shockwave(HDirection direction, TileSet *tileset)
{
    type = ENEMY_ATTACK;

    ts = tileset;
    anim = new Animation(ts, 0.1f, true);

    speed = 800.0f;

    if (direction == H_LEFT)
    {
        speed = -speed;
    }

    BBox(new Rect(-16.0f, -16.0f, 16.0f, 40.0f));
}

// ---------------------------------------------------------------------------------

Shockwave::~Shockwave()
{
    delete anim;
}

// ---------------------------------------------------------------------------------

void Shockwave::Update()
{
    Translate(speed * gameTime, 0);

    anim->NextFrame();
}

// ---------------------------------------------------------------------------------

void Shockwave::OnCollision(Object *obj)
{
    if (obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT)
    {
        TP2::scene->Delete(this, MOVING);
    }
}
