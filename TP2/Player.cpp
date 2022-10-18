#include "Player.h"
#include "Attack.h"
#include "EntityBlock.h"
#include "Fireball.h"
#include "Level2.h"
#include "Spike.h"
#include "TP2.h"
#include "Util.h"
#include "Wall.h"

Player::Player()
{
    type = PLAYER;

    hp = 6;

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
    uint dyingRight[2] = {13, 14};
    uint dyingLeft[2] = {37, 38};
    uint deadRight[1] = {15};
    uint deadLeft[1] = {39};
    uint respawnRight[2] = {15, 13};
    uint respawnLeft[2] = {39, 37};

    animation->Add(STILL * H_RIGHT, idleRight, 4);
    animation->Add(STILL * H_LEFT, idleLeft, 4);
    animation->Add(WALKING * H_RIGHT, walkRight, 2);
    animation->Add(WALKING * H_LEFT, walkLeft, 2);
    animation->Add(JUMPING * H_RIGHT, jumpRight, 1);
    animation->Add(JUMPING * H_LEFT, jumpLeft, 1);
    animation->Add(FALLING * H_RIGHT, fallRight, 2);
    animation->Add(FALLING * H_LEFT, fallLeft, 2);
    animation->Add(ATTACKING * H_RIGHT * RIGHT, attackRight, 2);
    animation->Add(ATTACKING * H_RIGHT * LEFT, attackRight, 2);
    animation->Add(ATTACKING * H_LEFT * LEFT, attackLeft, 2);
    animation->Add(ATTACKING * H_LEFT * RIGHT, attackLeft, 2);
    animation->Add(ATTACKING * H_LEFT * UP, attackUpLeft, 2);
    animation->Add(ATTACKING * H_RIGHT * UP, attackUpRight, 2);
    animation->Add(ATTACKING * H_LEFT * DOWN, attackDownLeft, 2);
    animation->Add(ATTACKING * H_RIGHT * DOWN, attackDownRight, 2);
    animation->Add(DASHING * H_RIGHT, dashRight, 2);
    animation->Add(DASHING * H_LEFT, dashLeft, 2);
    animation->Add(CASTING * H_RIGHT, castRight, 1);
    animation->Add(CASTING * H_LEFT, castLeft, 1);
    animation->Add(HURTING * H_RIGHT, hurtRight, 1);
    animation->Add(HURTING * H_LEFT, hurtLeft, 1);
    animation->Add(DYING * H_RIGHT, dyingRight, 2);
    animation->Add(DYING * H_LEFT, dyingLeft, 2);
    animation->Add(DEAD * H_RIGHT, deadRight, 1);
    animation->Add(DEAD * H_LEFT, deadLeft, 1);
    animation->Add(RESPAWNING * H_RIGHT, respawnRight, 2);
    animation->Add(RESPAWNING * H_LEFT, respawnLeft, 2);

    animation->Select(STILL * H_RIGHT);

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

bool Player::TakeDamage(uint damage, Direction dir)
{
    if (hurtCd.Down())
        return false;

    state = HURTING;

    hp = max(0, hp - damage);
    hurtCd.Restart();
    hurtAnimCd.Restart();

    if (dir == RIGHT)
    {
        direction = H_RIGHT;
        xSpeed = KNOCKBACK_SPEED;
        ySpeed = KNOCKBACK_UP_SPEED;
    }
    else if (dir == LEFT)
    {
        direction = H_LEFT;
        xSpeed = -KNOCKBACK_SPEED;
        ySpeed = KNOCKBACK_UP_SPEED;
    }
    else
    {
        xSpeed = direction == H_RIGHT ? KNOCKBACK_SPEED : -KNOCKBACK_SPEED;
        ySpeed = dir == UP ? KNOCKBACK_UP_SPEED : -KNOCKBACK_UP_SPEED;
    }

    TP2::audio->Play(PLAYER_HURT);

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

void Player::Knockback()
{
    if (attackDirection == DOWN)
        knockbackUpCd.Restart();
    else
        knockbackCd.Restart();
}

void Player::AddCooldowns(float dt)
{
    attackCd.Add(dt);
    attackAnimCd.Add(dt);
    healCd.Add(dt);
    fireballCd.Add(dt);
    fireballAnimCd.Add(dt);
    dashCd.Add(dt);
    dashAnimCd.Add(dt);
    hurtCd.Add(dt);
    hurtAnimCd.Add(dt);
    knockbackCd.Add(dt);
    knockbackUpCd.Add(dt);
    dyingCd.Add(dt);
    deadCd.Add(dt);
    respawnCd.Add(dt);
}

void Player::Respawn()
{
    xSpeed = 0.0f;
    ySpeed = 0.0f;

    state = RESPAWNING;

    FullHP();
    NoMana();

    attackCd.Restart();
    attackAnimCd.Restart();
    fireballCd.Restart();
    fireballAnimCd.Restart();
    dashCd.Restart();
    dashAnimCd.Restart();
    hurtCd.Restart();
    hurtAnimCd.Restart();
    knockbackCd.Restart();
    knockbackUpCd.Restart();
    dyingCd.Restart();
    deadCd.Restart();
    respawnCd.Restart();
}

void Player::TakeKnockback()
{
    float dx = xSpeed;
    float dy = ySpeed;

    if (attackDirection == DOWN)
    {
        dy = -1.0f * walkingSpeed;
        ySpeed = 0.0f;
    }
    else if (attackDirection == UP)
    {
        dy += 0.5f * walkingSpeed + gravity * gameTime;
        ySpeed = 0.75f * walkingSpeed;
    }
    else
    {
        ySpeed += gravity * gameTime;

        if (xSpeed == 0.0f)
        {
            if (attackDirection == LEFT)
                dx = 0.25f * walkingSpeed;
            else if (attackDirection == RIGHT)
                dx = -0.25f * walkingSpeed;
        }
        else
        {
            if (attackDirection == LEFT)
                dx += 1.25f * walkingSpeed;
            else if (attackDirection == RIGHT)
                dx -= 1.25f * walkingSpeed;
        }
    }

    Translate(dx * gameTime, dy * gameTime);
}

void Player::UpdateAnimation()
{
    uint oldAnimState = animation->Sequence();

    if (state == ATTACKING)
        animation->Select(ATTACKING * direction * attackDirection);
    else
        animation->Select(state * direction);

    if (oldAnimState != animation->Sequence())
    {
        animation->Restart();
    }

    switch (state)
    {
    case FALLING: {
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
        break;
    }
    case ATTACKING: {
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
        break;
    }
    case DASHING: {
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
        break;
    }
    case DYING: {
        if (dyingCd.Over(0.25f))
        {
            animation->Restart();
            animation->Frame(1);
        }
        else
        {
            animation->Restart();
            animation->Frame(0);
        }

        break;
    }
    case DEAD: {
        break;
    }
    case RESPAWNING: {
        if (respawnCd.Over(0.25f))
        {
            animation->Restart();
            animation->Frame(1);
        }
        else
        {
            animation->Restart();
            animation->Frame(0);
        }

        break;
    }
    default: {
        animation->NextFrame();
        break;
    }
    }
}

void Player::Update()
{
input : {
    PlayerState nextState = state;
    HDirection nextDirection = direction;

    if (state == STILL || state == WALKING || state == JUMPING || state == FALLING)
    {
        if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
        {
            xSpeed = 0.0f;
            if (direction == H_LEFT)
                nextDirection = H_LEFT;
            else if (direction == H_RIGHT)
                nextDirection = H_RIGHT;
        }
        else if (window->KeyDown(VK_LEFT))
        {
            xSpeed = -walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = H_LEFT;
        }
        else if (window->KeyDown(VK_RIGHT))
        {
            xSpeed = walkingSpeed;
            if (state == STILL || state == WALKING)
                nextState = WALKING;
            nextDirection = H_RIGHT;
        }
        else
        {
            xSpeed = 0.0f;
            if (state == STILL || state == WALKING)
            {
                nextState = STILL;
            }
        }

        // JUMP
        if (window->KeyDown('Z'))
        {
            if (jumpKeyCtrl && state != FALLING)
                TP2::audio->Play(PLAYER_JUMP);

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

            TP2::audio->Play(PLAYER_ATTACK);

            attackCd.Restart();
            attackAnimCd.Restart();

            attackKeyCtrl = false;

            if ((state == JUMPING || state == FALLING) && window->KeyDown(VK_DOWN))
                attackDirection = DOWN;
            else if (window->KeyDown(VK_UP))
                attackDirection = UP;
            else if (direction == H_LEFT)
                attackDirection = LEFT;
            else
                attackDirection = RIGHT;

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
                // TODO: Create fireball spawn animation

                TP2::audio->Play(PLAYER_FIREBALL);

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

                TP2::audio->Play(PLAYER_DASH);

                dashAnimCd.Restart();
                dashCd.Restart();

                dashGroundCtrl = false;
                dashKeyCtrl = false;
            }
            if (window->KeyUp('C'))
                dashKeyCtrl = true;
        }

        // HEAL
        if (window->KeyDown('A') && healKeyCtrl && HasMana() && healCd.Up())
        {
            healCd.Restart();
            // TODO: Create heal sound

            UseMana();

            hp = min(hp + 1, 6);

            healKeyCtrl = false;
        }
        if (window->KeyUp('A'))
            healKeyCtrl = true;
    }
    else if (state == ATTACKING)
    {
        if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_RIGHT))
        {
            xSpeed = 0.0f;
            if (direction == H_LEFT)
                nextDirection = H_LEFT;
            else if (direction == H_RIGHT)
                nextDirection = H_RIGHT;
        }
        else if (window->KeyDown(VK_LEFT))
        {
            xSpeed = -walkingSpeed;
            nextDirection = H_LEFT;
        }
        else if (window->KeyDown(VK_RIGHT))
        {
            xSpeed = walkingSpeed;
            nextDirection = H_RIGHT;
        }
        else
        {
            xSpeed = 0.0f;
            if (state == STILL || state == WALKING)
            {
                nextState = STILL;
            }
        }

        if (!jumpKeyCtrl && window->KeyUp('Z'))
        {
            jumpKeyCtrl = true;
            if (ySpeed < 0.0f)
                ySpeed = 0.0f;
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
        if (knockbackCd.Down() || knockbackUpCd.Down())
            TakeKnockback();

        dashGroundCtrl = true;

        nextState = FALLING;
        break;
    }
    case WALKING: {
        dashGroundCtrl = true;

        if (knockbackCd.Down() || knockbackUpCd.Down())
            TakeKnockback();
        else
            Translate(xSpeed * gameTime, 0.0f);

        nextState = FALLING;
        break;
    }
    case JUMPING: {
        if (knockbackCd.Down() || knockbackUpCd.Down())
            TakeKnockback();
        else
        {
            ySpeed += gravity * gameTime;
            Translate(xSpeed * gameTime, ySpeed * gameTime);
        }

        nextState = ySpeed >= 0.0f ? FALLING : JUMPING;
        break;
    }
    case FALLING: {
        landingCtrl = true;

        if (knockbackCd.Down() || knockbackUpCd.Down())
            TakeKnockback();
        else
        {
            ySpeed += gravity * gameTime;

            if (ySpeed > gravity * 2.0f)
                ySpeed = gravity * 2.0f;

            Translate(xSpeed * gameTime, ySpeed * gameTime);
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
            xSpeed = dashSpeed * (direction == H_LEFT ? -1.0f : 1.0f);
            Translate(xSpeed * gameTime, 0.0f);
            nextState = DASHING;
        }
        break;
    }
    case ATTACKING: {
        if (knockbackCd.Down() || knockbackUpCd.Down())
            TakeKnockback();
        else
        {
            ySpeed += gravity * gameTime;
            Translate(xSpeed * gameTime, ySpeed * gameTime);
        }

        if (attackAnimCd.Up())
            nextState = FALLING;
        else
            nextState = ATTACKING;
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
            if (direction == H_LEFT)
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
            {
                nextState = DYING;
                dyingCd.Restart();
            }
            else
                nextState = FALLING;
        }
        else
        {
            nextState = HURTING;
            Translate(xSpeed * gameTime, ySpeed * gameTime);
        }
        break;
    }
    case DYING: {
        ySpeed += gravity * gameTime;
        Translate(0.0f, ySpeed * gameTime);

        if (dyingCd.Up())
        {
            nextState = DEAD;
            deadCd.Restart();
        }
        else
            nextState = DYING;

        break;
    }
    case DEAD: {
        ySpeed += gravity * gameTime;
        Translate(0.0f, ySpeed * gameTime);
        if (deadCd.Up())
            TP2::playerDead = true;
        else
            nextState = DEAD;
        break;
    }
    case RESPAWNING: {
        if (respawnCd.Up())
        {
            nextState = STILL;
            ySpeed = 0.0f;
        }
        else
            nextState = RESPAWNING;
        break;
    }
    }

    switch (state)
    {
    case WALKING:
        if (walkingCtrl)
            TP2::audio->Play(PLAYER_WALK);
        walkingCtrl = false;
        break;
    default:
        TP2::audio->Stop(PLAYER_WALK);
        walkingCtrl = true;
        break;
    }

    AddCooldowns(gameTime);
}

    UpdateAnimation();

    state = nextState;
}

void Player::Draw()
{
    if (TP2::currentLevel != LEVELFINAL)
        light->Draw(round(x), round(y), LAYER_LIGHT);

    if (healCd.Down())
    {
        float g = 80.0f - 79.0f * healCd.Ratio();
        float b = 100.0f - 99.0f * healCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_PLAYER, 1.0f, 0.0f, {1.0f, g, b, 1.0f});
    }
    else
        animation->Draw(round(x), round(y), LAYER_PLAYER);
}

void Player::OnCollision(Object *other)
{
    Rect *self = (Rect *)BBox();

    switch (other->Type())
    {
    case WALL_TOP: {
        Rect *wallBBox = (Rect *)other->BBox();

        bool justEntered = oldBottom <= other->Y() && self->Bottom() >= other->Y();
        bool isInside = self->Right() != wallBBox->Left() && self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            if (landingCtrl)
            {
                landingCtrl = false;
                TP2::audio->Play(PLAYER_LAND);
            }

            MoveTo(x, other->Y() - self->bottom);
            if (state == FALLING)
            {
                state = STILL;
                ySpeed = 0.0f;
            }
        }

        break;
    }
    case WALL_BOTTOM: {
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
        if (Alive())
        {
            Entity *enemy = (Entity *)other;
            if (enemy->Alive())
                TakeDamage(1, enemy->X() > x ? LEFT : RIGHT);
        }
        break;
    }
    case ENEMY_ATTACK: {
        if (Alive())
            TakeDamage(1, other->X() > x ? LEFT : RIGHT);
        break;
    }
    case SPIKE: {
        if (Alive())
        {
            Spike *spike = (Spike *)other;
            Rect *wall = (Rect *)spike->BBox();

            if (self->Bottom() != wall->Top() && self->Top() != wall->Bottom() && self->Right() != wall->Left() &&
                self->Left() != wall->Right())
                TakeDamage(1, ((Spike *)other)->Dir());
        }
        break;
    }
    case ENTITY_BLOCK_BOSS_ACTIVATOR: {
        EntityBlockBossActivator *activator = (EntityBlockBossActivator *)other;
        if (activator->WhatBoss() == FALSE_KNIGHT)
        {
            Level2::fk->Activate();
            Level2::leftGate->Close();
        }

        TP2::scene->Delete(activator, STATIC);
    }
    }
}
