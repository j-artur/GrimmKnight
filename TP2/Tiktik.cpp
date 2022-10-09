#include "Tiktik.h"
#include "Attack.h"
#include "EntityBlock.h"
#include "Fireball.h"
#include "TP2.h"
#include "Wall.h"

Tiktik::Tiktik(TileSet *tileSet, int iX, int iY)
{
    type = ENEMY;

    hp = 8;

    animation = new Animation(tileSet, 0.3f, true);

    uint seqWalkRight[2] = {0, 1};
    uint seqWalkLeft[2] = {4, 5};
    uint seqHurtRight[1] = {2};
    uint seqHurtLeft[1] = {6};
    uint seqDeadRight[1] = {3};
    uint seqDeadLeft[1] = {7};

    animation->Add(TIKTIK_WALKING * RIGHT, seqWalkRight, 2);
    animation->Add(TIKTIK_WALKING * LEFT, seqWalkLeft, 2);
    animation->Add(TIKTIK_HURTING * RIGHT, seqHurtRight, 1);
    animation->Add(TIKTIK_HURTING * LEFT, seqHurtLeft, 1);
    animation->Add(TIKTIK_DEAD * RIGHT, seqDeadRight, 1);
    animation->Add(TIKTIK_DEAD * LEFT, seqDeadLeft, 1);

    animation->Select(TIKTIK_WALKING * RIGHT);

    BBox(new Rect(-14.0f, -4.0f, 14.0f, 16.0f));

    MoveTo(iX * 32.0f + 18.0f, iY * 32.0f + 18.0f);
}

Tiktik::~Tiktik()
{
    delete animation;
}

bool Tiktik::TakeDamage(uint damage, AttackDirection atkDir)
{
    if (state == TIKTIK_DEAD || state == TIKTIK_HURTING)
        return false;

    if (atkDir == ATK_LEFT)
    {
        xSpeed = -KNOCKBACK_SPEED;
        ySpeed = -KNOCKBACK_SPEED;
        direction = RIGHT;
    }
    else if (atkDir == ATK_RIGHT)
    {
        xSpeed = KNOCKBACK_SPEED;
        ySpeed = -KNOCKBACK_SPEED;
        direction = LEFT;
    }
    else if (atkDir == ATK_UP)
        ySpeed = KNOCKBACK_UP_SPEED;
    else if (atkDir == ATK_DOWN)
        ySpeed = KNOCKBACK_SPEED;

    hp -= damage;

    hurtCd.Restart();
    state = TIKTIK_HURTING;

    return true;
}

void Tiktik::Update()
{
    switch (state)
    {
    case TIKTIK_HURTING:
        if (hurtCd.Up())
        {
            if (hp <= 0)
            {
                dieCd.Restart();
                state = TIKTIK_DEAD;
            }
            else
                state = TIKTIK_WALKING;
        }
        break;
    case TIKTIK_DEAD:
        xSpeed = 0.0f;
        if (dieCd.Up())
            TP2::scene->Delete();
        break;
    case TIKTIK_WALKING:
        if (direction == LEFT)
            xSpeed = -walkSpeed;
        else
            xSpeed = walkSpeed;
        break;
    }

    hurtCd.Add(gameTime);
    dieCd.Add(gameTime);

    ySpeed += gravity * gameTime;

    Translate(xSpeed * gameTime, ySpeed * gameTime);

    animation->Select(state * direction);
    animation->NextFrame();
}

void Tiktik::Draw()
{
    if (state == TIKTIK_DEAD)
    {
        float f = 1.0f - dieCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_ENEMY, 1.0f, 0.0f, {f, f, f, f});
    }
    else
        animation->Draw(round(x), round(y), LAYER_ENEMY);
}

void Tiktik::OnCollision(Object *other)
{
    Rect *self = (Rect *)BBox();

    switch (other->Type())
    {
    case ENTITY_BLOCK_BOTTOM:
    case WALL_TOP: {
        Rect *wallBBox = (Rect *)other->BBox();
        if (ySpeed > 0.0f && self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right())
        {
            ySpeed = 0.0f;
            MoveTo(x, other->Y() - self->bottom);
        }
        break;
    }
    case ENTITY_BLOCK_TOP:
    case WALL_BOTTOM: {
        Rect *wallBBox = (Rect *)other->BBox();
        if (ySpeed < 0.0f && self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right())
        {
            ySpeed = 0.0f;
            MoveTo(x, other->Y() - self->top);
        }
        break;
    }
    case ENTITY_BLOCK_RIGHT:
    case WALL_LEFT: {
        Rect *wallBBox = (Rect *)other->BBox();
        if (self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom())
        {
            MoveTo(other->X() - self->right, y);
            direction = LEFT;
        }
        break;
    }
    case ENTITY_BLOCK_LEFT:
    case WALL_RIGHT: {
        Rect *wallBBox = (Rect *)other->BBox();
        if (self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom())
        {

            MoveTo(other->X() - self->left, y);
            direction = RIGHT;
        }
        break;
    }
    }
}
