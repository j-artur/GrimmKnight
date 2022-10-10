#include "ActionArea.h"

ActionArea::ActionArea()
{
    type = ENTITY_ACTION_AREA;

	BBox(new Rect(-192.0f, -100.0f, 192.0f, 16.0));
}

ActionArea::~ActionArea()
{
}

void ActionArea::Update()
{
}

void ActionArea::OnCollision(Object* other)
{
	if (other->Type() == PLAYER)
	{
		playerInside = true;
		playerDirection = other->X() < x ? LEFT : RIGHT;
	}
	else
		if (player->X() < x - 192.0f || player->X() > x + 192.0f)
			playerInside = false;
}

