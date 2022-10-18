#include "ActionArea.h"

ActionArea::ActionArea(float x1, float y1, float x2, float y2)
{
    type = ENTITY_ACTION_AREA;

    this->x1 = x1;
    this->x2 = x2;

    BBox(new Rect(x1, y1, x2, y2));
}

ActionArea::~ActionArea()
{
}

void ActionArea::Update()
{
}

void ActionArea::OnCollision(Object *other)
{
    if (other->Type() == PLAYER)
    {
        playerInside = true;
        playerDirection = other->X() < x ? H_LEFT : H_RIGHT;
    }
    else if (player->X() < x + x1 || player->X() > x + x2)
        playerInside = false;
}
