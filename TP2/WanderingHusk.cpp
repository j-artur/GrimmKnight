#include "WanderingHusk.h"
#include "Attack.h"
#include "EntityBlock.h"
#include "Fireball.h"
#include "TP2.h"
#include "Wall.h"

WanderingHusk::WanderingHusk(TileSet *tileSet, int iX, int iY)
{
    type = ENEMY;

    hp = 15;

    animation = new Animation(tileSet, 0.3f, true);

    actionArea = new ActionArea();
    actionArea->MoveTo(x, y);

    TP2::scene->Add(actionArea, MOVING);

    uint seqWalkRight[7] = {0, 1, 2, 3, 4, 5, 6};
    uint seqWalkLeft[7] = {0, 1, 2, 3, 4, 5, 6};
    uint seqPrepRight[5] = {7, 8, 9, 10, 11};
    uint seqPrepLeft[5] = {7, 8, 9, 10, 11};
    uint seqRunRight[4] = {14, 15, 16, 17};
    uint seqRunLeft[4] = {14, 15, 16, 17};
    uint seqHurtRight[1] = {11};
    uint seqHurtLeft[1] = {11};
    uint seqDeadRight[7] = {21, 22, 23, 24, 25, 26, 27};
    uint seqDeadLeft[7] = {21, 22, 23, 24, 25, 26, 27};

    animation->Add(WH_WALKING * H_RIGHT, seqWalkRight, 2);
    animation->Add(WH_WALKING * H_LEFT, seqWalkLeft, 2);
    animation->Add(WH_PRE_RUNNING * H_RIGHT, seqPrepRight, 2);
    animation->Add(WH_PRE_RUNNING * H_LEFT, seqPrepLeft, 2);
    animation->Add(WH_RUNNING * H_RIGHT, seqRunRight, 2);
    animation->Add(WH_RUNNING * H_LEFT, seqRunLeft, 2);
    animation->Add(WH_HURTING * H_RIGHT, seqHurtRight, 1);
    animation->Add(WH_HURTING * H_LEFT, seqHurtLeft, 1);
    animation->Add(WH_DEAD * H_RIGHT, seqDeadRight, 1);
    animation->Add(WH_DEAD * H_LEFT, seqDeadLeft, 1);

    animation->Select(WH_WALKING * H_RIGHT);

    BBox(new Rect(-14.0f, -4.0f, 14.0f, 16.0f));

    MoveTo(iX * 32.0f + 18.0f, iY * 32.0f + 18.0f);
}

WanderingHusk::~WanderingHusk()
{
    delete animation;
}

bool WanderingHusk::TakeDamage(uint damage, Direction atkDir)
{

    if (state == WH_DEAD || state == WH_HURTING)
        return false;

    if (atkDir == LEFT)
    {
        xSpeed = -KNOCKBACK_SPEED;
        ySpeed = -KNOCKBACK_SPEED;
        direction = H_RIGHT;
    }
    else if (atkDir == RIGHT)
    {
        xSpeed = KNOCKBACK_SPEED;
        ySpeed = -KNOCKBACK_SPEED;
        direction = H_LEFT;
    }
    else if (atkDir == UP)
        ySpeed = KNOCKBACK_UP_SPEED;
    else if (atkDir == DOWN)
        ySpeed = KNOCKBACK_SPEED;

    hp -= damage;

    hurtCd.Restart();
    state = WH_HURTING;

    return true;
}

void WanderingHusk::Update()
{
    actionArea->MoveTo(x, y);

    if (actionArea->IsPlayerInside() && runCd.Up() && state != WH_DEAD)
    {
        direction = actionArea->PlayerDirection();
        state = WH_PRE_RUNNING;
        chargeRunCd.Restart();
        runCd.Restart();
        animation->Restart();
        xSpeed = 0.0f;
    }

    switch (state)
    {
    case WH_HURTING:
        if (hurtCd.Up())
        {
            if (hp <= 0)
            {
                dieCd.Restart();
                state = WH_DEAD;
            }
            else
                state = WH_WALKING;
        }
        break;
    case WH_DEAD:
        xSpeed = 0.0f;
        if (dieCd.Up())
        {
            TP2::scene->Delete();
            TP2::scene->Delete(actionArea, MOVING);
        }
        break;
    case WH_WALKING:
        if (direction == H_LEFT)
            xSpeed = -walkSpeed;
        else
            xSpeed = walkSpeed;
        break;
    case WH_PRE_RUNNING:
        if (chargeRunCd.Up())
        {
            direction = TP2::player->X() < x ? H_LEFT : H_RIGHT;
            state = WH_RUNNING;
        }
        break;
    case WH_RUNNING:
        if (runCd.Down())
        {
            xSpeed = direction == H_LEFT ? -runningSpeed : runningSpeed;
        }
        else
            state = WH_WALKING;
        break;
    }

    hurtCd.Add(gameTime);
    dieCd.Add(gameTime);

    chargeRunCd.Add(gameTime);
    runCd.Add(gameTime);

    ySpeed += gravity * gameTime;

    Translate(xSpeed * gameTime, ySpeed * gameTime);

    animation->Select(state * direction);
    animation->NextFrame();
}

void WanderingHusk::Draw()
{
    if (state == WH_DEAD)
    {
        float f = 1.0f - dieCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_ENEMY, 1.0f, 0.0f, {f, f, f, f});
    }
    else
        animation->Draw(round(x), round(y), LAYER_ENEMY);
}

void WanderingHusk::OnCollision(Object *other)
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
            direction = H_LEFT;
        }
        break;
    }
    case ENTITY_BLOCK_LEFT:
    case WALL_RIGHT: {
        Rect *wallBBox = (Rect *)other->BBox();
        if (self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom())
        {

            MoveTo(other->X() - self->left, y);
            direction = H_RIGHT;
        }
        break;
    }
    }
}
