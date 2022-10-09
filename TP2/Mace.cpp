#include "Mace.h"
#include "Util.h"

Mace::Mace()
{
    type = ENEMY_ATTACK;

    BBox(new Rect(-36, -36, 36, 36));
}

Mace::~Mace()
{
}

void Mace::Update()
{
}
