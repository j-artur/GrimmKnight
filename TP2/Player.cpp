#include "Player.h"

Player::Player()
{
    type = PLAYER;

    tileSet = new TileSet("Resources/Player.png", 36, 68, 6, 12);
    animation = new Animation(tileSet, 0.20f, true);
    light = new Sprite("Resources/Light.png");

    uint idleRight[4] = {0, 1, 2, 3};
    uint idleLeft[4] = {6, 7, 8, 9};
    uint walkRight[2] = {4, 5};
    uint walkLeft[2] = {10, 11};

    animation->Add(IDLE_RIGHT, idleRight, 4);
    animation->Add(IDLE_LEFT, idleLeft, 4);
    animation->Add(WALK_RIGHT, walkRight, 2);
    animation->Add(WALK_LEFT, walkLeft, 2);

    state = IDLE_RIGHT;

    BBox(new Rect(-14.0f, -30.0f, 14.0f, 30.0f));
}

Player::~Player()
{
    delete tileSet;
    delete animation;
}

void Player::Update()
{
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

    if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
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
        xSpeed = -maxXSpeed;
        state = WALK_LEFT;
    }
    else if (window->KeyDown(VK_RIGHT))
    {
        xSpeed = +maxXSpeed;
        state = WALK_RIGHT;
    }

    ySpeed += gravity * gameTime;

    if (ySpeed > gravity)
        ySpeed = gravity;
    if (ySpeed < jumpingSpeed)
        ySpeed = jumpingSpeed;

    Translate(xSpeed * gameTime, ySpeed * gameTime);

    standing = false;

    animation->Select(state);
    animation->NextFrame();
}

void Player::Draw()
{
    light->Draw(float((int)x), float((int)y), Layer::MIDDLE);
    animation->Draw(float((int)x), float((int)y), Layer::UPPER);
}

void Player::OnCollision(Object *other)
{
    switch (other->Type())
    {
    case WALL_TOP: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;

        if (oldBottom <= wall->Y() && self->Bottom() >= wall->Y())
        {
            MoveTo(x, other->Y() - self->bottom - 0.1f);
            standing = true;
        }
        break;
    }
    case WALL_BOTTOM: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;

        if (oldTop >= wall->Y() && self->Top() <= wall->Y())
        {
            MoveTo(x, other->Y() - self->top + 0.1f);
            ySpeed = 0.0f;
        }
        break;
    }
    case WALL_LEFT: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;

        if (oldRight <= wall->X() && self->Right() >= wall->X())
        {
            MoveTo(other->X() - self->right - 0.1f, y);
        }
        break;
    }
    case WALL_RIGHT: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;

        if (oldLeft >= wall->X() && self->Left() <= wall->X())
        {
            MoveTo(other->X() - self->left + 0.1f, y);
        }
        break;
    }
    }
}
