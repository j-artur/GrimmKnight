#include "FalseKnight.h"
#include "Barrel.h"
#include "Shockwave.h"
#include "TP2.h"
#include "Wall.h"

FalseKnight::FalseKnight(int iX, int iY)
{
    type = ENEMY;

    hp = armorHealth;

    mace = new Mace();
    TP2::scene->Add(mace, MOVING);

    random_device rd;
    rng.seed(rd());
    cd = uniform_int_distribution<int>(1, 3);
    move = uniform_int_distribution<int>(1, 3);
    nextMove = 2;
    betweenAttacksCd.Restart();

    tileSet = new TileSet("Resources/FalseKnight.png", 452, 260, 6, 24);
    shockwaveTileSet = new TileSet("Resources/attack.png", 64, 64, 3, 4);
    barrelSprite = new Sprite("Resources/WIP/rock.png");
    animation = new Animation(tileSet, 0.2f, true);

    uint idleRight[2] = {0, 1};
    uint idleLeft[2] = {12, 13};
    uint leapRight[2] = {3, 4};
    uint leapLeft[2] = {15, 16};
    uint prepSlamRight[1] = {5};
    uint prepSlamLeft[1] = {17};
    uint attackRight[2] = {6, 7};
    uint attackLeft[2] = {18, 19};
    uint bludgeonRight[2] = {8, 9};
    uint bludgeonLeft[2] = {20, 21};
    uint stunLeft[2] = {10, 11};
    uint stunRight[2] = {22, 23};
    uint inactive[1] = {23};

    animation->Add(FK_IDLE * H_LEFT, idleLeft, 2);
    animation->Add(FK_IDLE * H_RIGHT, idleRight, 2);
    animation->Add(FK_PREP_SLAM * H_LEFT, prepSlamLeft, 1);
    animation->Add(FK_PREP_SLAM * H_RIGHT, prepSlamRight, 1);
    animation->Add(FK_SLAMMING * H_LEFT, attackLeft, 2);
    animation->Add(FK_SLAMMING * H_RIGHT, attackRight, 2);
    animation->Add(FK_LEAPING * H_LEFT, leapLeft, 2);
    animation->Add(FK_LEAPING * H_RIGHT, leapRight, 2);
    animation->Add(FK_BLUDGEONING * H_RIGHT, bludgeonRight, 2);
    animation->Add(FK_BLUDGEONING * H_LEFT, bludgeonLeft, 2);
    animation->Add(FK_STUN * H_RIGHT, stunRight, 2);
    animation->Add(FK_STUN * H_LEFT, stunLeft, 2);
    animation->Add(FK_INACTIVE, inactive, 1);

    MoveTo(iX * 32.0f, iY * 32.0f);

    animation->Select(state * direction);
    BBox(new Rect(-64.0f, -16.0f, 64.0f, 124.0f));
}

FalseKnight::~FalseKnight()
{
    delete tileSet;
    delete animation;
    delete shockwaveTileSet;
    delete barrelSprite;
}

bool FalseKnight::TakeDamage(uint damage, Direction dir)
{
    if (hurtCd.Up())
    {
        if (isStunned)
        {
            hp -= damage;
            TP2::audio->Play(SFK_HEAD_DAMAGE);
            return true;
        }
        else
        {
            currentArmorHealth -= damage;
            TP2::audio->Play(SFK_ARMOR_DAMAGE);
            return false;
        }

        hurtCd.Restart();
    }

    return false;
}

void FalseKnight::Activate()
{
    state = FK_IDLE;
    TP2::audio->Play(SFK_THEME, true);
}

void FalseKnight::Update()
{
    // DIRECTION CONTROLS
    direction = TP2::player->X() < x ? H_LEFT : H_RIGHT;
    directionMult = direction == H_LEFT ? -1.0f : 1.0f;

    mace->MoveTo(x - 120.0f * directionMult, y);

    if (betweenAttacksCd.Over(attackCd) && !isAttacking && state == FK_IDLE)
    {
        isAttacking = true;

        strikeCtrl = true;
        voiceCtrl = true;

        switch (nextMove)
        {
        case 1:
            state = FK_LEAP;
            break;
        case 2:
            slamCd.Restart();
            prepSlamCd.Restart();
            state = FK_SLAM;
            break;
        case 3:
            state = FK_BLUDGEON;
            break;
        case 4:
            state = FK_RAGE;
            break;
        default:
            break;
        }
    }

    // LEAP AND BLUDGEON
    if (state == FK_LEAP || state == FK_BLUDGEON)
    {
        mace->MoveTo(x - 120.0f * attackDirection, y);
        if (!isJumping)
        {
            TP2::audio->Play(SFK_JUMP);

            attackDirection = direction == H_LEFT ? -1.0f : 1.0f;

            animation->Select(state == FK_LEAP ? FK_LEAPING : FK_BLUDGEONING * direction);
            animation->Restart();

            JumpTo(J_PLAYER);
        }

        if (state == FK_BLUDGEON)
        {
            switch (animation->Frame())
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                mace->MoveTo(x - 150.0f * attackDirection, y + 50.0f);
                break;
            case 6:
                if (voiceCtrl)
                {
                    voiceCtrl = false;
                    TP2::audio->Play(SFK_SWING);
                    if (canKill)
                        TP2::audio->Play(SFK_VOICE_ATTACK);
                }
                mace->MoveTo(x - 110.0f * attackDirection, y - 100.0f);
                break;
            case 7:
                mace->MoveTo(x + 100.0f * attackDirection, y + 50.0f);
                break;
            case 8:
                mace->MoveTo(x - 110.0f * attackDirection, y - 100.0f);
                break;
            case 9:
                if (strikeCtrl)
                {
                    strikeCtrl = false;
                    TP2::audio->Play(SFK_STRIKE_GROUND);
                }
                mace->MoveTo(x - 130.0f * attackDirection, y - 70.0f);
                break;
            default:
                break;
            }
        }

        if (jumpCd.Up())
        {

            if (state == FK_LEAP)
            {
                TP2::audio->Play(SFK_LAND);
                slamCd.Restart();
                prepSlamCd.Restart();
                state = FK_SLAM;
            }
            else
            {
                isAttacking = false;
                betweenAttacksCd.Restart();
                attackCd = cd(rng);
                nextMove = move(rng);

                state = FK_IDLE;
            }
            isJumping = false;
        }

        jumpCd.Add(gameTime);
    }

    // SLAM
    if (state == FK_SLAM)
    {
        uint oldAnimState = animation->Sequence();
        if (!prepSlamCd.Up())
        {
            animation->Select(FK_PREP_SLAM * direction);
            mace->MoveTo(x - 150.0f * directionMult, y + 50.0f);
        }
        else
        {
            animation->Select(FK_SLAMMING * direction);
            animation->Delay(0.1f);
            // fix hitboxes when we have sprites
            switch (animation->Frame())
            {
            case 0:
                mace->MoveTo(x - 50.0f * directionMult, y - 140.0f);
                break;
            case 1:
                mace->MoveTo(x + 150.0f * directionMult, y - 50.0f);
                break;
            case 2:
                mace->MoveTo(x + 160.0f * directionMult, y + 50.0f);
                if (!spawnedShockwave)
                {
                    TP2::audio->Play(SFK_STRIKE_GROUND);
                    Shockwave *shockwave = new Shockwave(direction, shockwaveTileSet);
                    shockwave->MoveTo(mace->X(), mace->Y());
                    TP2::scene->Add(shockwave, MOVING);
                    spawnedShockwave = true;
                }
                break;
            case 3:
                mace->MoveTo(x, y - 140.0f);
                break;
            case 4:
                mace->MoveTo(x - 40.0f * directionMult, y - 140.0f);
                break;
            case 5:
                mace->MoveTo(x - 60.0f * directionMult, y - 110.0f);
                break;
            case 6:
                mace->MoveTo(x - 40.0f * directionMult, y - 80.0f);
                break;
            case 7:
                mace->MoveTo(x - 120.0f * directionMult, y);
                break;

            default:
                break;
            }
        }

        if (slamCd.Up())
        {
            isAttacking = false;
            betweenAttacksCd.Restart();
            attackCd = cd(rng);
            nextMove = move(rng);

            spawnedShockwave = false;
            animation->Delay(0.2f);
            state = FK_IDLE;
        }

        if (oldAnimState != animation->Sequence())
        {
            animation->Restart();
        }

        slamCd.Add(gameTime);
        prepSlamCd.Add(gameTime);
    }

    // RAGE
    if (state == FK_RAGE)
    {
        mace->MoveTo(x - 120.0f * attackDirection, y);
        if (!isJumping)
        {
            attackDirection = direction == H_LEFT ? -1.0f : 1.0f;

            animation->Select(FK_LEAPING * direction);
            animation->Restart();

            JumpTo(J_MIDDLE);
        }

        if (jumpCd.Up() && !rageStarted)
        {
            TP2::audio->Play(SFK_VOICE_RAGE);
            rageStarted = true;
            attackRageCd.Restart();
            rageCd.Restart();
        }

        if (rageStarted)
        {

            if (attackRageCd.Up())
            {
                attackDirection = -attackDirection;
                direction = direction == H_LEFT ? H_LEFT : H_RIGHT;
                spawnedBarrels = false;
                attackRageCd.Restart();
            }

            animation->Select(FK_SLAMMING * direction);
            animation->Delay(0.1f);
            // fix hitboxes when we have sprites
            switch (animation->Frame())
            {
            case 0:
                mace->MoveTo(x - 50.0f * attackDirection, y - 140.0f);
                break;
            case 1:
                mace->MoveTo(x + 150.0f * attackDirection, y - 50.0f);
                break;
            case 2:
                mace->MoveTo(x + 160.0f * attackDirection, y + 50.0f);
                if (!spawnedBarrels)
                {
                    spawnedBarrels = true;
                    Sprite *sp = new Sprite("Resources/WIP/rock.png");

                    Barrel *b1 = new Barrel(sp);
                    Barrel *b2 = new Barrel(sp);
                    // Barrel* b3 = new Barrel(sp);

                    TP2::scene->Add(b1, MOVING);
                    TP2::scene->Add(b2, MOVING);
                    // TP2::scene->Add(b3, MOVING);
                }
                break;
            case 3:
                mace->MoveTo(x, y - 140.0f);
                break;
            case 4:
                mace->MoveTo(x - 40.0f * attackDirection, y - 140.0f);
                break;
            case 5:
                mace->MoveTo(x - 60.0f * attackDirection, y - 110.0f);
                break;
            case 6:
                mace->MoveTo(x - 40.0f * attackDirection, y - 80.0f);
                break;
            case 7:
                mace->MoveTo(x - 120.0f * attackDirection, y);
                break;

            default:
                break;
            }

            if (rageCd.Up())
            {
                isAttacking = false;
                betweenAttacksCd.Restart();
                attackCd = cd(rng);
                nextMove = move(rng);

                animation->Delay(0.2f);
                spawnedBarrels = false;
                rageStarted = false;
                state = FK_IDLE;
            }
        }

        attackRageCd.Add(gameTime);
        rageCd.Add(gameTime);
        jumpCd.Add(gameTime);
    }

    if (state == FK_STUN)
    {
        mace->MoveTo(x, y);
        if (!isStunned)
        {
            headOutCd.Restart();
            stunCd.Restart();

            ySpeed = 0;

            animation->Select(FK_STUN); // change to rolling and head going out of the armor?

            isStunned = true;
        }

        if (headOutCd.Up() && isStunned)
        {
            animation->Select(FK_STUN); // stunned animation with the head out

            if (stunCd.Up() || hp <= 0)
            {
                if (!canKill)
                {
                    currentArmorHealth = armorHealth;
                    hp = headHealth;
                    canKill = true;
                }

                isStunned = false;
                state = FK_RAGE;
            }
        }

        headOutCd.Add(gameTime);
        stunCd.Add(gameTime);
    }

    if (currentArmorHealth <= 0)
    {
        state = FK_STUN;
    }

    if (canKill)
    {
        move = uniform_int_distribution<int>(1, 4);
        if (hp <= 0)
        {
            // state = FK_DEAD;
            TP2::scene->Delete();
            TP2::scene->Delete(mace, MOVING);
            if (voiceCtrl)
            {
                TP2::audio->Stop(SFK_THEME);
                // play boss killing music
            }
        }
    }

    // IDLE
    if (state == FK_IDLE)
    {
        animation->Select(state);
    }

    // GRAVITY ACCELERATION AND MOVEMENT
    ySpeed += gravity * gameTime;
    Translate(xSpeed * gameTime, ySpeed * gameTime);

    betweenAttacksCd.Add(gameTime);

    animation->NextFrame();
}

void FalseKnight::Draw()
{
    animation->Draw(round(x), round(y), LAYER_BOSS);
}

void FalseKnight::OnCollision(Object *other)
{
    switch (other->Type())
    {
    case WALL_TOP: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = self->Bottom() >= wall->Y();
        bool isInside = self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            MoveTo(x, other->Y() - self->bottom);
            ySpeed = 0.0f;
            xSpeed = 0.0f;
        }
        break;
    }
    case WALL_LEFT: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = self->Right() >= wall->X();
        bool isInside = self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->right, y);
            xSpeed = 0.0f;
        }
        break;
    }
    case WALL_RIGHT: {
        Rect *self = (Rect *)BBox();
        Wall *wall = (Wall *)other;
        Rect *wallBBox = (Rect *)wall->BBox();

        bool justEntered = self->Left() <= wall->X();
        bool isInside = self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->left, y);
            xSpeed = 0.0f;
        }
        break;
    }
    }
}

void FalseKnight::JumpTo(FK_JumpTo jumpToLocation)
{
    jumpCd.Restart();
    isJumping = true;
    if (voiceCtrl && state == FK_LEAP)
    {
        voiceCtrl = false;
        if (canKill)
            TP2::audio->Play(SFK_VOICE_ATTACK);
    }

    float timeJumping = state == FK_BLUDGEON ? 1.4f : 2.0f;
    float jumpingTo = jumpToLocation == PLAYER ? TP2::player->X() : window->CenterX();

    ySpeed = state == FK_BLUDGEON ? bludgeonSpeed : leapSpeed;
    xSpeed = (jumpingTo - x) / timeJumping;
}
