#include "Attack.h"
#include "TP2.h"
#include "Util.h"

Attack::Attack(TileSet *tileSet, Player *player, AttackDirection direction)
{
    type = ATTACK;

	anim = new Animation(tileSet, 0.1f, false);

	uint seqLeft[2] = {0, 1};
	uint seqRight[2] = {2, 3};
	uint seqDown[2] = {4, 5};
	uint seqUp[2] = {6, 7};

	anim->Add(ATK_LEFT, seqLeft, 2);
	anim->Add(ATK_RIGHT, seqRight, 2);
	anim->Add(ATK_UP, seqUp, 2);
	anim->Add(ATK_DOWN, seqDown, 2);

	anim->Select(direction);

    this->player = player;
    this->direction = direction;

    int width = tileSet->TileWidth();
    int height = tileSet->TileHeight();

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
}

// ---------------------------------------------------------------------------------

Attack::~Attack()
{
    delete anim;
}

// ---------------------------------------------------------------------------------

void Attack::Update()
{
    switch (direction)
    {
    case ATK_UP:
        MoveTo(player->X(), player->Y() - 64.0f);
        break;
    case ATK_DOWN:
        MoveTo(player->X(), player->Y() + 64.0f);
        break;
    case ATK_LEFT:
        MoveTo(player->X() - 32.0f, player->Y());
        break;
    case ATK_RIGHT:
        MoveTo(player->X() + 32.0f, player->Y());
        break;
    }

    anim->NextFrame();
    if (anim->Inactive())
        TP2::scene->Delete();
}
