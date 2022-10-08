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
        ySpeed = KNOCKBACK_SPEED;
        direction = RIGHT;
    }
    else if (atkDir == ATK_RIGHT)
    {
        xSpeed = KNOCKBACK_SPEED;
        ySpeed = KNOCKBACK_SPEED;
        direction = LEFT;
    }
    else if (atkDir == ATK_UP)
        ySpeed = KNOCKBACK_UP_SPEED;
    else if (atkDir == ATK_DOWN)
        ySpeed = 2 * KNOCKBACK_SPEED;

    hp -= damage;

    if (hp <= 0)
    {
        dieCd.Restart();
        state = TIKTIK_DEAD;
        animation->Select(TIKTIK_DEAD * direction);
        xSpeed = 0.0f;
    }
    else
    {
        hurtCd.Restart();
        state = TIKTIK_HURTING;
        animation->Select(TIKTIK_HURTING * direction);
    }

    return true;
}

void Tiktik::Update()
{
    switch (state)
    {
    case TIKTIK_HURTING:
        if (hurtCd.Up())
            state = TIKTIK_WALKING;
        break;
    case TIKTIK_DEAD:
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
    animation->Draw(round(x), round(y), LAYER_ENEMY);
}

void Tiktik::OnCollision(Object *other)
{
    Rect *self = (Rect *)BBox();

    switch (other->Type())
    {
    case ENTITY_BLOCK_TOP:
    case WALL_TOP:
        if (ySpeed >= 0.0f)
        {
            ySpeed = 0.0f;
            MoveTo(x, other->Y() - self->bottom);
        }
        break;
    case ENTITY_BLOCK_BOTTOM:
    case WALL_BOTTOM:
        if (ySpeed <= 0.0f)
        {
            ySpeed = 0.0f;
            MoveTo(x, other->Y() - self->top);
        }
        break;
    case ENTITY_BLOCK_LEFT:
    case WALL_LEFT:
        MoveTo(other->X() - self->right, y);
        direction = LEFT;
        break;
    case ENTITY_BLOCK_RIGHT:
    case WALL_RIGHT:
        MoveTo(other->X() - self->left, y);
        direction = RIGHT;
        break;
    }
}
