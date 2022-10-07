#include "Player.h"
#include "Attack.h"
#include "Fireball.h"
#include "TP2.h"
#include "Wall.h"
#include <cmath>

Player::Player()
{
    type = PLAYER;

    tileSet = new TileSet("Resources/Player.png", 100, 68, 5, 40);
    animation = new Animation(tileSet, 0.20f, true);

    attackTileSet = new TileSet("Resources/Attack.png", 80, 80, 4, 8);

    uint idleRight[4] = {0, 5, 10, 15};
    uint idleLeft[4] = {20, 25, 30, 35};
    uint walkRight[2] = {1, 6};
    uint walkLeft[2] = {21, 26};
    uint jumpRight[1] = {2};
    uint jumpLeft[2] = {22};
    uint fallRight[2] = {7, 12};
    uint fallLeft[2] = {27, 32};
    uint attackRight[1] = {17};
    uint attackLeft[1] = {37};
    uint dashRight[2] = {11, 16};
    uint dashLeft[2] = {31, 36};

    animation->Add(STILL * RIGHT, idleRight, 4);
    animation->Add(STILL * LEFT, idleLeft, 4);
    animation->Add(WALKING * RIGHT, walkRight, 2);
    animation->Add(WALKING * LEFT, walkLeft, 2);
    animation->Add(JUMPING * RIGHT, jumpRight, 1);
    animation->Add(JUMPING * LEFT, jumpLeft, 1);
    animation->Add(FALLING * RIGHT, fallRight, 2);
    animation->Add(FALLING * LEFT, fallLeft, 2);
    animation->Add(ATTACKING * ATK_RIGHT * RIGHT, attackRight, 1);
    animation->Add(ATTACKING * ATK_LEFT * LEFT, attackLeft, 1);
    animation->Add(DASHING * RIGHT, dashRight, 2);
    animation->Add(DASHING * LEFT, dashLeft, 2);

    light = new Sprite("Resources/Light.png");

    Rect *self = new Rect(-14.0f, -30.0f, 14.0f, 30.0f);
    BBox(self);

    oldTop = self->Top();
    oldBottom = self->Bottom();
    oldLeft = self->Left();
    oldRight = self->Right();
}

Player::~Player()
{
    delete tileSet;
    delete animation;
    delete light;
}

void Player::Update()
{
input:
{
    PlayerState nextState = state;
    Direction nextDirection = direction;

    if (state == STILL || state == WALKING || state == JUMPING || state == FALLING)
    {
        if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
        {
            xSpeed = 0.0f;
            if (direction == LEFT)
                nextDirection = LEFT;
            else if (direction == RIGHT)
                nextDirection = RIGHT;
        }
        else if (window->KeyDown(VK_LEFT))
        {
            xSpeed = -walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = LEFT;
        }
        else if (window->KeyDown(VK_RIGHT))
        {
            xSpeed = walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = RIGHT;
        }
        else
        {
            xSpeed = 0.0f;
            if (state == STILL || state == WALKING)
            {
                nextState = STILL;
            }
        }

        if (window->KeyDown('Z'))
        {
            if ((state == STILL || state == WALKING) && jumpKeyCtrl)
            {
                nextState = JUMPING;
                ySpeed = jumpingSpeed;
            }

            jumpKeyCtrl = false;
        }
        else
        {
            jumpKeyCtrl = true;

            if (state == JUMPING)
            {
                ySpeed = 0.0f;
                nextState = FALLING;
            }
        }

        // ATTACK
        if (window->KeyDown('X') && attackCd.Ready() && attackKeyCtrl)
        {
            // TODO: Create attack animation

            nextState = ATTACKING;

            attackCd.Reset();
            attackAnimCd.Reset();

            attackKeyCtrl = false;

            if ((state == JUMPING || state == FALLING) && window->KeyDown(VK_DOWN))
                attackDirection = ATK_DOWN;
            else if (window->KeyDown(VK_UP))
                attackDirection = ATK_UP;
            else if (direction == LEFT)
                attackDirection = ATK_LEFT;
            else
                attackDirection = ATK_RIGHT;

            Attack *atk = new Attack(attackTileSet, this, attackDirection);
            TP2::scene->Add(atk, MOVING);
        }
        if (window->KeyUp('X'))
            attackKeyCtrl = true;

        // FIREBALL
        if (window->KeyDown('S') && fireballCd.Ready() && fireballKeyCtrl && HasMana())
        {
            // TODO: Create fireball animation

            // nextState = CASTING;

            UseMana();
            fireballCd.Reset();

            fireballKeyCtrl = false;

            Fireball *fb = new Fireball(this, direction);
            TP2::scene->Add(fb, MOVING);
        }
        if (window->KeyUp('S'))
            fireballKeyCtrl = true;

        // DASH
        if (window->KeyDown('C') && dashCd.Ready() && dashKeyCtrl && dashGroundCtrl)
        {
            // TODO: Create dash animation

            nextState = DASHING;

            dashAnimCd.Reset();
            dashCd.Reset();

            dashGroundCtrl = false;
            dashKeyCtrl = false;
        }

        if (window->KeyUp('C'))
            dashKeyCtrl = true;
    }
    else if (state == ATTACKING)
    {
        if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
        {
            xSpeed = 0.0f;
            if (direction == LEFT)
                nextDirection = LEFT;
            else if (direction == RIGHT)
                nextDirection = RIGHT;
        }
        else if (window->KeyDown(VK_LEFT))
        {
            xSpeed = -walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = LEFT;
        }
        else if (window->KeyDown(VK_RIGHT))
        {
            xSpeed = walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = RIGHT;
        }
        else
        {
            xSpeed = 0.0f;
            if (state == STILL || state == WALKING)
            {
                nextState = STILL;
            }
        }
    }

    state = nextState;
    direction = nextDirection;
}

    PlayerState nextState = state;

update:
{
    if (!canMove)
    {
        attackCd.Add(0.1f * gameTime);
        attackAnimCd.Add(0.1f * gameTime);
        fireballCd.Add(0.1f * gameTime);
        dashCd.Add(0.1f * gameTime);
        dashAnimCd.Add(0.1f * gameTime);
        return;
    }

    // update old position
    Rect *self = (Rect *)BBox();
    oldTop = self->Top();
    oldBottom = self->Bottom();
    oldLeft = self->Left();
    oldRight = self->Right();

    switch (state)
    {
    case STILL:
    {
        dashGroundCtrl = true;

        nextState = FALLING;
        break;
    }
    case WALKING:
    {

        dashGroundCtrl = true;

        Translate(xSpeed * gameTime, 0.0f);

        nextState = FALLING;
        break;
    }
    case JUMPING:
    {
        ySpeed += gravity * gameTime;

        Translate(xSpeed * gameTime, ySpeed * gameTime);

        nextState = ySpeed >= 0.0f ? FALLING : JUMPING;
        break;
    }
    case FALLING:
    {
        ySpeed += gravity * gameTime;

        if (ySpeed > gravity * 2.0f)
            ySpeed = gravity * 2.0f;

        Translate(xSpeed * gameTime, ySpeed * gameTime);

        if (ySpeed >= 224.0f)
        {
            animation->Restart();
            animation->Frame(1);
        }

        nextState = FALLING;
        break;
    }
    case DASHING:
    {
        if (dashAnimCd.Ready())
        {
            nextState = FALLING;
            ySpeed = 0.0f;
        }
        else
        {
            xSpeed = dashSpeed * (direction == LEFT ? -1.0f : 1.0f);
            Translate(xSpeed * gameTime, 0.0f);
            nextState = DASHING;
            if (dashAnimCd.Elapsed(0.2f))
            {
                animation->Restart();
                animation->Frame(0);
            }
            else if (dashAnimCd.Elapsed(0.05f))
            {
                animation->Restart();
                animation->Frame(1);
            }
            else
            {
                animation->Restart();
                animation->Frame(0);
            }
        }
        break;
    }
    case ATTACKING:
    {
        ySpeed += gravity * gameTime;
        Translate(xSpeed * gameTime, ySpeed * gameTime);

        if (attackAnimCd.Ready())
            nextState = FALLING;
        else
            nextState = ATTACKING;

        break;
    }
    case CASTING:
    {
        break;
    }
    case HURTING:
    {
        break;
    }
    case DYING:
    {
        break;
    }
    case RESPAWNING:
    {
        break;
    }
    }

    attackCd.Add(gameTime);
    attackAnimCd.Add(gameTime);
    fireballCd.Add(gameTime);
    dashCd.Add(gameTime);
    dashAnimCd.Add(gameTime);
}

    // if (state == ATTACKING)
    //     animation->Select(ATTACKING * direction * attackDirection);
    // else
    //     animation->Select(state * direction);

    uint oldAnimState = animation->Sequence();

    if (state != CASTING && state != HURTING && state != DYING && state != RESPAWNING)
    {
        if (state == ATTACKING)
        {
            if (attackDirection == ATK_UP || attackDirection == ATK_DOWN)
            {
                if (direction == LEFT)
                    animation->Select(ATTACKING * ATK_LEFT * LEFT);
                else
                    animation->Select(ATTACKING * ATK_RIGHT * RIGHT);
            }
            else
            {
                if (direction == LEFT)
                    animation->Select(ATTACKING * ATK_LEFT * LEFT);
                else
                    animation->Select(ATTACKING * ATK_RIGHT * RIGHT);
            }
        }
        else
        {
            animation->Select(state * direction);
        }
    }
    else
    {
        if (xSpeed != 0.0f)
            animation->Select(WALKING * direction);
        else
            animation->Select(STILL * direction);
    }

    if (oldAnimState != animation->Sequence())
    {
        animation->Restart();
    }

    animation->NextFrame();

    state = nextState;
}

void Player::Draw()
{
    light->Draw(round(x), round(y), 0.95f);
    animation->Draw(round(x), round(y), Layer::MIDDLE);
}

void Player::OnCollision(Object *other)
{
    switch (other->Type())
    {
    case WALL_TOP:
    {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = oldBottom <= wall->Y() && self->Bottom() >= wall->Y();
        bool isInside = self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            MoveTo(x, other->Y() - self->bottom);
            ySpeed = 0.0f;
            if (state == FALLING)
                state = STILL;
        }
        break;
    }
    case WALL_BOTTOM:
    {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = oldTop >= wall->Y() && self->Top() <= wall->Y();
        bool isInside = self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            MoveTo(x, other->Y() - self->top);
            ySpeed = 0.0f;
        }
        break;
    }
    case WALL_LEFT:
    {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = oldRight <= wall->X() && self->Right() >= wall->X();
        bool isInside = self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->right, y);
            dashAnimCd.Left(0.1f);
        }
        break;
    }
    case WALL_RIGHT:
    {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = oldLeft >= wall->X() && self->Left() <= wall->X();
        bool isInside = self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->left, y);
            dashAnimCd.Left(0.1f);
        }
        break;
    }
    }
}
