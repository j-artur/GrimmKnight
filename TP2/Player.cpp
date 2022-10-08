#include "Player.h"
#include "Attack.h"
#include "Fireball.h"
#include "TP2.h"
#include "Util.h"
#include "Wall.h"

Player::Player()
{
    type = PLAYER;

    hp = 5;

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
    uint hurtRight[1] = {5};
    uint hurtLeft[1] = {29};
    uint dieRight[3] = {13, 14, 15};
    uint dieLeft[3] = {37, 38, 39};

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
    animation->Add(HURTING * RIGHT, hurtRight, 1);
    animation->Add(HURTING * LEFT, hurtLeft, 1);
    animation->Add(DYING * RIGHT, dieRight, 3);
    animation->Add(DYING * LEFT, dieLeft, 3);

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

bool Player::TakeDamage(uint damage, AttackDirection dir)
{
    if (hurtCd.Down())
        return false;

    state = HURTING;

    hp = max(0, hp - damage);
    hurtCd.Restart();
    hurtAnimCd.Restart();

    if (dir == ATK_RIGHT)
    {
        direction = RIGHT;
        xSpeed = KNOCKBACK_SPEED;
        ySpeed = KNOCKBACK_UP_SPEED;
    }
    else if (dir == ATK_LEFT)
    {
        direction = LEFT;
        xSpeed = -KNOCKBACK_SPEED;
        ySpeed = KNOCKBACK_UP_SPEED;
    }
    else
    {
        xSpeed = direction == RIGHT ? KNOCKBACK_SPEED : -KNOCKBACK_SPEED;
        ySpeed = dir == ATK_UP ? KNOCKBACK_UP_SPEED : -KNOCKBACK_UP_SPEED;
    }
    return true;
}

bool Player::HasMana()
{
    return mana >= 3;
}

void Player::UseMana()
{
    mana -= 3;
}

void Player::AddMana()
{
    if (mana < 9)
        mana++;
}

void Player::AddCooldowns(float dt)
{
    attackCd.Add(dt);
    attackAnimCd.Add(dt);
    fireballCd.Add(dt);
    fireballAnimCd.Add(dt);
    dashCd.Add(dt);
    dashAnimCd.Add(dt);
    hurtCd.Add(dt);
    hurtAnimCd.Add(dt);
}

void Player::Knockback()
{
    knockbackCd.Restart();
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
        if (window->KeyDown('X') && attackCd.Up() && attackKeyCtrl)
        {
            nextState = ATTACKING;

            attackCd.Restart();
            attackAnimCd.Restart();

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
        if (fireball)
        {
            if (window->KeyDown('S') && fireballCd.Up() && fireballKeyCtrl && HasMana())
            {
                // TODO: Create fireball animation

                xSpeed = 0.0f;
                ySpeed = 0.0f;
                nextState = CASTING;

                UseMana();
                fireballCd.Restart();
                fireballAnimCd.Restart();

                fireballKeyCtrl = false;

                Fireball *fb = new Fireball(this, direction);
                TP2::scene->Add(fb, MOVING);
            }
            if (window->KeyUp('S'))
                fireballKeyCtrl = true;
        }

        // DASH
        if (dash)
        {
            if (window->KeyDown('C') && dashCd.Up() && dashKeyCtrl && dashGroundCtrl)
            {
                nextState = DASHING;

                dashAnimCd.Restart();
                dashCd.Restart();

                dashGroundCtrl = false;
                dashKeyCtrl = false;
            }
            if (window->KeyUp('C'))
                dashKeyCtrl = true;
        }
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
        if (dashAnimCd.Up())
        {
            nextState = FALLING;
            ySpeed = 0.0f;
        }
        else
        {
            xSpeed = dashSpeed * (direction == LEFT ? -1.0f : 1.0f);
            Translate(xSpeed * gameTime, 0.0f);
            nextState = DASHING;
            if (dashAnimCd.Over(0.225f))
            {
                animation->Restart();
                animation->Frame(0);
            }
            else if (dashAnimCd.Over(0.025f))
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
        if (knockbackCd.Down())
        {
            float dx = xSpeed;
            float dy = ySpeed;

            if (attackDirection == ATK_DOWN)
            {
                dy = -1.5f * walkingSpeed;
                ySpeed = 0.0f;
            }
            else if (attackDirection == ATK_UP)
            {
                dy += 0.5f * walkingSpeed + gravity * gameTime;
                ySpeed = 0.75f * walkingSpeed;
            }
            else
            {
                ySpeed += gravity * gameTime;

                if (xSpeed == 0.0f)
                {
                    if (attackDirection == ATK_LEFT)
                        dx = 0.25f * walkingSpeed;
                    else if (attackDirection == ATK_RIGHT)
                        dx = -0.25f * walkingSpeed;
                }
                else
                {
                    if (attackDirection == ATK_LEFT)
                        dx += 1.25f * walkingSpeed;
                    else if (attackDirection == ATK_RIGHT)
                        dx -= 1.25f * walkingSpeed;
                }
            }

            Translate(dx * gameTime, dy * gameTime);
            knockbackCd.Add(gameTime);
        }
        else
        {
            ySpeed += gravity * gameTime;
            Translate(xSpeed * gameTime, ySpeed * gameTime);
        }

        if (attackAnimCd.Up())
            nextState = FALLING;
        else
        {
            nextState = ATTACKING;
            if (attackAnimCd.Over(0.075f))
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
        if (fireballAnimCd.Up())
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
        if (hurtAnimCd.Up())
        {
            ySpeed = 0.0f;

            if (hp <= 0)
                nextState = DYING;
            else
                nextState = FALLING;
        }
        else
        {
            nextState = HURTING;
            Translate(xSpeed * gameTime, ySpeed * gameTime);
        }
    }
    case DYING: {
        break;
    }
    case RESPAWNING: {
        break;
    }
    }

    AddCooldowns(gameTime);
}

    uint oldAnimState = animation->Sequence();

    if (state == ATTACKING)
        animation->Select(ATTACKING * direction * attackDirection);
    else
        animation->Select(state * direction);

    if (oldAnimState != animation->Sequence())
    {
        animation->Restart();
    }

    animation->NextFrame();

    state = nextState;
}

void Player::Draw()
{
    light->Draw(round(x), round(y), LAYER_LIGHT);
    animation->Draw(round(x), round(y), LAYER_PLAYER);
}

void Player::OnCollision(Object *other)
{
    switch (other->Type())
    {
    case WALL_TOP: {
        Rect *self = (Rect *)BBox();
        Rect *wallBBox = (Rect *)other->BBox();

        bool justEntered = oldBottom <= other->Y() && self->Bottom() >= other->Y();
        bool isInside = self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            MoveTo(x, other->Y() - self->bottom);
            if (state == FALLING)
                state = STILL;
        }
        break;
    }
    case WALL_BOTTOM: {
        Rect *self = (Rect *)BBox();
        Rect *wallBBox = (Rect *)other->BBox();

        bool justEntered = oldTop >= other->Y() && self->Top() <= other->Y();
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
        Rect *wallBBox = (Rect *)other->BBox();

        bool justEntered = oldRight <= other->X() && self->Right() >= other->X();
        bool isInside = self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->right, y);
            dashAnimCd.Leave(0.1f);
        }
        break;
    }
    case WALL_RIGHT: {
        Rect *self = (Rect *)BBox();
        Rect *wallBBox = (Rect *)other->BBox();

        bool justEntered = oldLeft >= other->X() && self->Left() <= other->X();
        bool isInside = self->Bottom() != wallBBox->Top() && self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->left, y);
            dashAnimCd.Leave(0.1f);
        }
        break;
    }
    case ENEMY: {
        Entity *enemy = (Entity *)other;
        if (enemy->Alive())
            TakeDamage(1, enemy->X() > x ? ATK_LEFT : ATK_RIGHT);
        break;
    }
    case ENEMY_ATTACK: {
        TakeDamage(1, other->X() > x ? ATK_LEFT : ATK_RIGHT);
        break;
    }
    }
}
