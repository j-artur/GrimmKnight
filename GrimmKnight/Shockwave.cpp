#include "Shockwave.h"
#include "GrimmKnight.h"
#include "Util.h"

Shockwave::Shockwave(Geometry *mace, HDirection direction, TileSet *tileset)
{
    type = ENEMY_ATTACK;

    ts = tileset;
    anim = new Animation(ts, 0.1f, true);

    uint seqRight[2] = {0, 1};
    uint seqLeft[2] = {2, 3};

    anim->Add(H_RIGHT, seqRight, 2);
    anim->Add(H_LEFT, seqLeft, 2);

    anim->Select(direction);

    speed = 800.0f;

    if (direction == H_LEFT)
    {
        speed = -speed;
    }

    BBox(new Rect(-16.0f, -4.0f, 16.0f, 60.0f));

    MoveTo(mace->X(), mace->Y() - 20.0f);
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
        GrimmKnight::scene->Delete(this, MOVING);
    }
}
