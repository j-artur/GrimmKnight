#include "Player.h"
#include "Attack.h"
#include "Fireball.h"
#include "TP2.h"
#include "Wall.h"
#include <cmath>

Player::Player()
{
    type = PLAYER;

    tileSet = new TileSet("Resources/Player.png", 100, 68, 8, 48);
    animation = new Animation(tileSet, 0.20f, true);

    attackTileSet = new TileSet("Resources/Attack.png", 120, 120, 4, 8);

    uint idleRight[4] = {0, 1, 2, 3};
    uint idleLeft[4] = {24, 25, 26, 27};
    uint walkRight[2] = {8, 9};
    uint walkLeft[2] = {32, 33};
    uint jumpRight[1] = {4};
    uint jumpLeft[1] = {28};
    uint fallRight[2] = {16, 17};
    uint fallLeft[2] = {40, 41};
    uint attackRight[2] = {18, 19};
    uint attackLeft[2] = {42, 43};
    uint attackUpRight[2] = {20, 21};
    uint attackUpLeft[2] = {44, 45};
    uint attackDownRight[2] = {22, 23};
    uint attackDownLeft[2] = {46, 47};
    uint dashRight[2] = {10, 11};
    uint dashLeft[2] = {34, 35};
    uint castRight[1] = {12};
    uint castLeft[1] = {36};

    animation->Add(STILL * RIGHT, idleRight, 4);
    animation->Add(STILL * LEFT, idleLeft, 4);
    animation->Add(WALKING * RIGHT, walkRight, 2);
    animation->Add(WALKING * LEFT, walkLeft, 2);
    animation->Add(JUMPING * RIGHT, jumpRight, 1);
    animation->Add(JUMPING * LEFT, jumpLeft, 1);
    animation->Add(FALLING * RIGHT, fallRight, 2);
    animation->Add(FALLING * LEFT, fallLeft, 2);
    animation->Add(ATTACKING * ATK_RIGHT * LEFT, attackRight, 2);
    animation->Add(ATTACKING * ATK_RIGHT * RIGHT, attackRight, 2);
    animation->Add(ATTACKING * ATK_LEFT * LEFT, attackLeft, 2);
    animation->Add(ATTACKING * ATK_LEFT * RIGHT, attackLeft, 2);
    animation->Add(ATTACKING * ATK_UP * LEFT, attackUpLeft, 2);
    animation->Add(ATTACKING * ATK_UP * RIGHT, attackUpRight, 2);
    animation->Add(ATTACKING * ATK_DOWN * LEFT, attackDownLeft, 2);
    animation->Add(ATTACKING * ATK_DOWN * RIGHT, attackDownRight, 2);
    animation->Add(DASHING * RIGHT, dashRight, 2);
    animation->Add(DASHING * LEFT, dashLeft, 2);
    animation->Add(CASTING * RIGHT, castRight, 1);
    animation->Add(CASTING * LEFT, castLeft, 1);

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
    if (!canMove)
    {
        dashCd.Add(0.1f * gameTime);
        dashAnimCd.Add(0.1f * gameTime);
        return;
    }

input : {
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

            Attack *atk = new Attack(attackTileSet, this, direction, attackDirection);
            TP2::scene->Add(atk, MOVING);
        }
        if (window->KeyUp('X'))
            attackKeyCtrl = true;

        // FIREBALL
        if (window->KeyDown('S') && fireballCd.Ready() && fireballKeyCtrl /* && HasMana() */)
        {
            // TODO: Create fireball animation

            xSpeed = 0.0f;
            ySpeed = 0.0f;
            nextState = CASTING;

            // UseMana();
            fireballCd.Reset();
            fireballAnimCd.Reset();

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

update : {

    // update old position
    Rect *self = (Rect *)BBox();
    oldTop = self->Top();
    oldBottom = self->Bottom();
    oldLeft = self->Left();
    oldRight = self->Right();

    switch (state)
    {
    case STILL: {
        dashGroundCtrl = true;

        nextState = FALLING;
        break;
    }
    case WALKING: {

        dashGroundCtrl = true;

        Translate(xSpeed * gameTime, 0.0f);

        nextState = FALLING;
        break;
    }
    case JUMPING: {
        ySpeed += gravity * gameTime;

        Translate(xSpeed * gameTime, ySpeed * gameTime);

        nextState = ySpeed >= 0.0f ? FALLING : JUMPING;
        break;
    }
    case FALLING: {
        ySpeed += gravity * gameTime;

        if (ySpeed > gravity * 2.0f)
            ySpeed = gravity * 2.0f;

        Translate(xSpeed * gameTime, ySpeed * gameTime);

        if (ySpeed >= 224.0f)
        {
            animation->Restart();
            animation->Frame(1);
        }
        else
        {
            animation->Restart();
            animation->Frame(0);
        }

        nextState = FALLING;
        break;
    }
    case DASHING: {
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
            if (dashAnimCd.Elapsed(0.225f))
            {
                animation->Restart();
                animation->Frame(0);
            }
            else if (dashAnimCd.Elapsed(0.025f))
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
    case ATTACKING: {
        ySpeed += gravity * gameTime;
        Translate(xSpeed * gameTime, ySpeed * gameTime);

        if (attackAnimCd.Ready())
            nextState = FALLING;
        else
        {
            nextState = ATTACKING;
            if (attackAnimCd.Elapsed(0.075f))
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
    case CASTING: {
        if (fireballAnimCd.Ready())
        {
            ySpeed += gravity * gameTime;
            nextState = FALLING;
        }
        else
        {
            if (direction == LEFT)
                Translate(48.0f * gameTime, ySpeed * gameTime);
            else
                Translate(-48.0f * gameTime, ySpeed * gameTime);
            nextState = CASTING;
        }

        break;
    }
    case HURTING: {
        break;
    }
    case DYING: {
        break;
    }
    case RESPAWNING: {
        break;
    }
    }

    attackCd.Add(gameTime);
    attackAnimCd.Add(gameTime);
    fireballCd.Add(gameTime);
    fireballAnimCd.Add(gameTime);
    dashCd.Add(gameTime);
    dashAnimCd.Add(gameTime);
}

    if (state == ATTACKING)
        animation->Select(ATTACKING * direction * attackDirection);
    else
        animation->Select(state * direction);

    uint oldAnimState = animation->Sequence();

    // if (state != HURTING && state != DYING && state != RESPAWNING)
    // {
    //     if (state == ATTACKING)
    //     {
    //         if (attackDirection == ATK_UP || attackDirection == ATK_DOWN)
    //         {
    //             if (direction == LEFT)
    //                 animation->Select(ATTACKING * ATK_LEFT * LEFT);
    //             else
    //                 animation->Select(ATTACKING * ATK_RIGHT * RIGHT);
    //         }
    //         else
    //         {
    //             if (direction == LEFT)
    //                 animation->Select(ATTACKING * ATK_LEFT * LEFT);
    //             else
    //                 animation->Select(ATTACKING * ATK_RIGHT * RIGHT);
    //         }
    //     }
    //     else
    //     {
    //         animation->Select(state * direction);
    //     }
    // }
    // else
    // {
    //     if (xSpeed != 0.0f)
    //         animation->Select(WALKING * direction);
    //     else
    //         animation->Select(STILL * direction);
    // }

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
    case WALL_TOP: {
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
    case WALL_BOTTOM: {
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
    case WALL_LEFT: {
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
    case WALL_RIGHT: {
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
