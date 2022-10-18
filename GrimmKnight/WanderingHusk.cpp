#include "WanderingHusk.h"
#include "Attack.h"
#include "EntityBlock.h"
#include "Fireball.h"
#include "GrimmKnight.h"
#include "Wall.h"

WanderingHusk::WanderingHusk(TileSet *tileSet, int iX, int iY, uint voiceId)
{
    type = ENEMY;

    hp = 15;

    this->voiceId = voiceId;

    animation = new Animation(tileSet, 0.3f, true);

    actionArea = new ActionArea(-192.0f, -100.0f, 192.0f, 24.0f);
    actionArea->MoveTo(x, y);

    GrimmKnight::scene->Add(actionArea, MOVING);

    uint seqWalkLeft[2] = {0, 3};
    uint seqWalkRight[2] = {6, 9};
    uint seqRunLeft[2] = {1, 4};
    uint seqRunRight[2] = {7, 10};
    uint seqHurtLeft[1] = {2};
    uint seqHurtRight[1] = {8};
    uint seqDeadRight[7] = {5};
    uint seqDeadLeft[7] = {11};

    animation->Add(WH_WALKING * H_LEFT, seqWalkLeft, 2);
    animation->Add(WH_WALKING * H_RIGHT, seqWalkRight, 2);
    animation->Add(WH_PRE_RUNNING * H_LEFT, seqWalkLeft, 2);
    animation->Add(WH_PRE_RUNNING * H_RIGHT, seqWalkRight, 2);
    animation->Add(WH_RUNNING * H_LEFT, seqRunLeft, 2);
    animation->Add(WH_RUNNING * H_RIGHT, seqRunRight, 2);
    animation->Add(WH_HURTING * H_LEFT, seqHurtLeft, 1);
    animation->Add(WH_HURTING * H_RIGHT, seqHurtRight, 1);
    animation->Add(WH_DEAD * H_LEFT, seqDeadLeft, 1);
    animation->Add(WH_DEAD * H_RIGHT, seqDeadRight, 1);

    animation->Select(WH_WALKING * H_RIGHT);

    BBox(new Rect(-18.0f, -18.0f, 18.0f, 24.0f));

    MoveTo(iX * 32.0f + 26.0f, iY * 32.0f + 8.0f);
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
                GrimmKnight::audio->Play(ENEMY_DEATH);
                state = WH_DEAD;
            }
            else
                state = WH_WALKING;
        }
        break;
    case WH_DEAD:
        GrimmKnight::audio->Stop(voiceId);
        xSpeed = 0.0f;
        if (dieCd.Up())
        {
            GrimmKnight::scene->Delete();
            GrimmKnight::scene->Delete(actionArea, MOVING);
        }
        break;
    case WH_WALKING:

        if (audioCtrl)
        {
            audioCtrl = false;
            GrimmKnight::audio->Frequency(voiceId, 1.25f);
            GrimmKnight::audio->Play(voiceId, true);
        }

        if (direction == H_LEFT)
            xSpeed = -walkSpeed;
        else
            xSpeed = walkSpeed;
        break;
    case WH_PRE_RUNNING:
        GrimmKnight::audio->Stop(voiceId);
        if (chargeRunCd.Up())
        {
            audioCtrl = true;
            runCtrl = true;
            direction = GrimmKnight::player->X() < x ? H_LEFT : H_RIGHT;
            GrimmKnight::audio->Play(ENEMY_RUN);
            state = WH_RUNNING;
        }
        break;
    case WH_RUNNING:
        if (runCd.Down())
        {
            if (runCtrl)
            {
                runCtrl = false;
                GrimmKnight::audio->Play(voiceId, true);
                GrimmKnight::audio->Frequency(voiceId, 2.0f);
            }
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
    else if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_ENEMY, 1.0f, 0.0f, {f, f, f, 1.0f});
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
