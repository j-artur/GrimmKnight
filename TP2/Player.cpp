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

    animation->Add(STILL * RIGHT, idleRight, 4);
    animation->Add(STILL * LEFT, idleLeft, 4);
    animation->Add(WALKING * RIGHT, walkRight, 2);
    animation->Add(WALKING * LEFT, walkLeft, 2);

    light = new Sprite("Resources/Light.png");

    state = FALLING;
    direction = RIGHT;

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

void Player::Input()
{
}

void Player::Update()
{
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
                nextState = STILL;
        }

        if (window->KeyDown('Z') && (state == STILL || state == WALKING))
        {
            nextState = JUMPING;
            ySpeed = jumpingSpeed;
        }

        if (state == JUMPING && window->KeyUp('Z'))
        {
            ySpeed = 0.0f;
            nextState = FALLING;
        }

        if (window->KeyDown('X') && attackCd.Ready())
        {
            // TODO: Create attack animation

            // nextState = ATTACKING;

            attackCd.Reset();

            if ((state == JUMPING || state == FALLING) && window->KeyDown(VK_DOWN))
                attackDirection = ATK_DOWN;
            else if (window->KeyDown(VK_UP))
                attackDirection = ATK_UP;
            else if (direction == LEFT)
                attackDirection = ATK_LEFT;
            else
                attackDirection = ATK_RIGHT;

            Attack *atk = new Attack(this, attackDirection);
            TP2::scene->Add(atk, MOVING);
        }

        if (window->KeyDown('C') && fireballCd.Ready())
        {
            // TODO: Create fireball animation

            // nextState = CASTING;

            fireballCd.Reset();

            Fireball *fb = new Fireball(this, direction);
            TP2::scene->Add(fb, MOVING);
        }

        if (window->KeyDown('V') && dashCd.Ready() && dashKeyCtrl && dashGroundCtrl)
        {
            // TODO: Create dash animation

            nextState = DASHING;

            dashingCd.Reset();
            dashCd.Reset();

            dashGroundCtrl = false;
            dashKeyCtrl = false;
        }

        if (window->KeyUp('V'))
            dashKeyCtrl = true;
    }

    state = nextState;
    direction = nextDirection;
}

update : {
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

    PlayerState nextState = state;

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

        nextState = FALLING;
        break;
    }
    case DASHING: {
        if (dashingCd.Ready())
        {
            nextState = FALLING;
            ySpeed = 0.0f;
        }
        else
        {
            xSpeed = dashSpeed * (direction == LEFT ? -1.0f : 1.0f);
            Translate(xSpeed * gameTime, 0.0f);
            nextState = DASHING;
        }
        break;
    }
    case ATTACKING: {
        break;
    }
    case CASTING: {
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

    state = nextState;
}

    attackCd.Add(gameTime);
    fireballCd.Add(gameTime);
    dashCd.Add(gameTime);
    dashingCd.Add(gameTime);

    // if (state == ATTACKING)
    //     animation->Select(ATTACKING * direction * attackDirection);
    // else
    //     animation->Select(state * direction);

    if (xSpeed != 0.0f)
        animation->Select(WALKING * direction);
    else
        animation->Select(STILL * direction);

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
