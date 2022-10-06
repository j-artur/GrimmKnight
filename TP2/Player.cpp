#include "Player.h"
#include "Attack.h"
#include "Fireball.h"
#include "TP2.h"
#include <cmath>

Player::Player()
{
    type = PLAYER;

    tileSet = new TileSet("Resources/Player.png", 36, 68, 6, 12);
    animation = new Animation(tileSet, 0.20f, true);

    uint idleRight[4] = {0, 1, 2, 3};
    uint idleLeft[4] = {6, 7, 8, 9};
    uint walkRight[2] = {4, 5};
    uint walkLeft[2] = {10, 11};

    animation->Add(IDLE_RIGHT, idleRight, 4);
    animation->Add(IDLE_LEFT, idleLeft, 4);
    animation->Add(WALK_RIGHT, walkRight, 2);
    animation->Add(WALK_LEFT, walkLeft, 2);

    light = new Sprite("Resources/Light.png");

    state = IDLE_RIGHT;

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
        attackCd.Add(0.1f * gameTime);
        fireballCd.Add(0.1f * gameTime);
        dashingCd.Add(0.1f * gameTime);
        dashCd.Add(0.1f * gameTime);
        return;
    }

    // update old position
    Rect *self = (Rect *)BBox();

    oldTop = self->Top();
    oldBottom = self->Bottom();
    oldLeft = self->Left();
    oldRight = self->Right();

    if (standing)
    {
        ySpeed = 0.0f;
        if (window->KeyDown('Z'))
        {
            if (!jumping)
            {
                jumping = true;
                ySpeed = jumpingSpeed;
            }
        }
    }
    if (window->KeyUp('Z'))
    {
        if (jumping)
            ySpeed = -ySpeed / 4.0f;
        jumping = false;
    }

    if (ySpeed >= 0.0f)
        jumping = false;

    // dash movement
    if (dashing)
    {
        xSpeed = 0.0f;
        ySpeed = 0.0f;
        Translate(dashSpeed * gameTime, 0);
    }
    // walking command
    else if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
    {
        xSpeed = 0.0f;
        switch (state)
        {
        case WALK_LEFT:
            state = IDLE_RIGHT;
            break;
        case WALK_RIGHT:
            state = IDLE_LEFT;
            break;
        }
    }
    else if (window->KeyUp(VK_LEFT) && window->KeyUp(VK_RIGHT))
    {
        xSpeed = 0.0f;
        switch (state)
        {
        case WALK_LEFT:
            state = IDLE_LEFT;
            break;
        case WALK_RIGHT:
            state = IDLE_RIGHT;
            break;
        }
    }
    else if (window->KeyDown(VK_LEFT))
    {
        xSpeed = -walkingSpeed;
        state = WALK_LEFT;
    }
    else if (window->KeyDown(VK_RIGHT))
    {
        xSpeed = +walkingSpeed;
        state = WALK_RIGHT;
    }

    // attack command
    if (attackCd.Ready())
    {
        if (window->KeyDown('X'))
        {
            attackCd.Reset();

            if (!standing && window->KeyDown(VK_DOWN))
                attackDirection = DOWN;
            else if (window->KeyDown(VK_UP))
                attackDirection = UP;
            else if (facing == F_LEFT)
                attackDirection = LEFT;
            else
                attackDirection = RIGHT;

            Attack *atk = new Attack(this, attackDirection);
            TP2::scene->Add(atk, MOVING);
        }
    }
    else
    {
        attackCd.Add(gameTime);
    }

    // fireball command
    if (fireballCd.Ready())
    {
        if (window->KeyDown('C'))
        {
            fireballCd.Reset();

            Fireball *fb = new Fireball(this, facing);
            TP2::scene->Add(fb, MOVING);
        }
    }
    else
    {
        fireballCd.Add(gameTime);
    }

    // dash command
    if (dashCd.Ready())
    {
        if (window->KeyDown('V') && dashKeyCtrl && dashGroundCtrl)
        {
            dashingCd.Reset();
            dashCd.Reset();

            dashGroundCtrl = false;
            dashKeyCtrl = false;
            dashing = true;
            dashSpeed = facing ? 720.0f : -720.0f;
        }
    }
    else
        dashCd.Add(gameTime);

    if (dashingCd.Ready())
        dashing = false;
    else
        dashingCd.Add(gameTime);

    if (window->KeyUp('V'))
        dashKeyCtrl = true;
    if (standing)
        dashGroundCtrl = true;

    ySpeed += gravity * gameTime;

    if (ySpeed > gravity)
        ySpeed = gravity;
    if (ySpeed < jumpingSpeed)
        ySpeed = jumpingSpeed;

    // updates what direction character is facing
    if (state == WALK_LEFT || state == IDLE_LEFT)
        facing = F_LEFT;
    else
        facing = F_RIGHT;

    if (attacking)
    {
        // select attack or fireball anim based on attacking or fireballing vars and keep on it until it finishes
    } // else
    // select default anim
    if (!dashing)
    {
        Translate(xSpeed * gameTime, ySpeed * gameTime);
    }

    standing = false;

    animation->Select(state);
    animation->NextFrame();
}

void Player::Draw()
{
    light->Draw(round(x), round(y), 0.95f);
    animation->Draw(round(x), round(y), Layer::UPPER);
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
            standing = true;
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
            dashingCd.Left(0.1f);
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
            dashingCd.Left(0.1f);
        }
        break;
    }
    }
}
