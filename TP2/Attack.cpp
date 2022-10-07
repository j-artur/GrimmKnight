#include "Attack.h"
#include "TP2.h"
#include "Util.h"

Attack::Attack(TileSet *tileSet, Player *player, Direction dir, AttackDirection atkDir)
{
    type = ATTACK;

    anim = new Animation(tileSet, 0.075f, false);

    uint seqLeft[2] = {0, 1};
    uint seqRight[2] = {2, 3};
    uint seqDownLeft[2] = {4, 5};
    uint seqDownRight[2] = {6, 7};
    uint seqUpLeft[2] = {8, 9};
    uint seqUpRight[2] = {10, 11};

    anim->Add(ATK_LEFT * LEFT, seqLeft, 2);
    anim->Add(ATK_LEFT * RIGHT, seqLeft, 2);
    anim->Add(ATK_RIGHT * LEFT, seqRight, 2);
    anim->Add(ATK_RIGHT * RIGHT, seqRight, 2);
    anim->Add(ATK_DOWN * LEFT, seqDownLeft, 2);
    anim->Add(ATK_DOWN * RIGHT, seqDownRight, 2);
    anim->Add(ATK_UP * LEFT, seqUpLeft, 2);
    anim->Add(ATK_UP * RIGHT, seqUpRight, 2);

    anim->Select(dir * atkDir);

    this->player = player;
    this->direction = atkDir;

    switch (dir)
    {
    case ATK_LEFT:
        BBox(new Rect(-60.0f, -40.0f, 40.0f, 40.0f));
        break;
    case ATK_RIGHT:
        BBox(new Rect(-40.0f, -40.0f, 60.0f, 40.0f));
        break;
    case ATK_UP:
        BBox(new Rect(-40.0f, -60.0f, 40.0f, 50.0f));
        break;
    case ATK_DOWN:
        BBox(new Rect(-40.0f, -50.0f, 40.0f, 60.0f));
        break;
    }
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
        MoveTo(player->X(), player->Y() - 48.0f);
        break;
    case ATK_DOWN:
        MoveTo(player->X(), player->Y() + 48.0f);
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
