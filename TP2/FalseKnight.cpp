#include "FalseKnight.h"
#include "Shockwave.h"
#include "Barrel.h"
#include "TP2.h"
#include "Wall.h"

FalseKnight::FalseKnight()
{
    type = ENEMY;

    hp = armorHealth;

    mace = new Mace();
    TP2::scene->Add(mace, MOVING);

    random_device rd;
    rng.seed(rd());
    cd = uniform_int_distribution<int>(2, 4);
    move = uniform_int_distribution<int>(1, 4);
    nextMove = 2;
    betweenAttacksCd.Restart();

    tileSet = new TileSet("Resources/WIP/FalseKnight.png", 303, 192, 5, 35);
    shockwaveTileSet = new TileSet("Resources/attack.png", 64, 64, 5, 10);
    barrelSprite = new Sprite("Resources/WIP/rock.png");
    animation = new Animation(tileSet, 0.2f, true);

    uint idleRight[5] = { 0, 1, 2, 3, 4 };
    uint idleLeft[5] = { 0, 1, 2, 3, 4 };
    uint leapRight[10] = { 5,6,7,8,9,10,11,12,13,14 };
    uint leapLeft[10] = { 5,6,7,8,9,10,11,12,13,14 };
    uint prepSlamRight[6] = {15, 16, 17, 18, 19, 20};
    uint prepSlamLeft[6] = {15, 16, 17, 18, 19, 20};
    uint attackRight[8] = {21, 22, 23, 24, 25, 26, 27, 28};
    uint attackLeft[8] = {21, 22, 23, 24, 25, 26, 27, 28};
    uint bludgeonRight[10] = { 29,30,31,32,33,34,35,36,37,38 };
    uint bludgeonLeft[10] = { 29,30,31,32,33,34,35,36,37,38 };

    animation->Add(FK_IDLE * LEFT, idleLeft, 5);
    animation->Add(FK_IDLE * RIGHT, idleRight, 5);
    animation->Add(FK_PREP_SLAM * LEFT, prepSlamLeft, 6);
    animation->Add(FK_PREP_SLAM * RIGHT, prepSlamRight, 6);
    animation->Add(FK_SLAMMING * LEFT, attackLeft, 8);
    animation->Add(FK_SLAMMING * RIGHT, attackRight, 8);
    animation->Add(FK_LEAPING * LEFT, leapLeft, 10);
    animation->Add(FK_LEAPING * RIGHT, leapRight, 10);
    animation->Add(FK_BLUDGEONING * RIGHT, bludgeonRight, 10);
    animation->Add(FK_BLUDGEONING * LEFT, bludgeonLeft, 10);

    animation->Select(state * direction);
    BBox(new Rect(-80, -192 / 2, 150, 192 / 2));
}

FalseKnight::~FalseKnight()
{
    delete tileSet;
    delete animation;
}

bool FalseKnight::TakeDamage(uint damage, AttackDirection dir)
{
    return true;
}

void FalseKnight::Update()
{
    // DIRECTION CONTROLS
    direction = TP2::player->X() < x ? LEFT : RIGHT;
    directionMult = direction == LEFT ? -1.0f : 1.0f;

    mace->MoveTo(x - 120.0f * directionMult, y);

    if (betweenAttacksCd.Over(attackCd) && !isAttacking) {
        isAttacking = true;
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
            attackDirection = direction == LEFT ? -1.0f : 1.0f;

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
                mace->MoveTo(x - 110.0f * attackDirection, y - 100.0f);
                break;
            case 7:
                mace->MoveTo(x + 100.0f * attackDirection, y + 50.0f);
                break;
            case 8:
                mace->MoveTo(x - 110.0f * attackDirection, y - 100.0f);
                break;
            case 9:
                mace->MoveTo(x - 130.0f * attackDirection, y - 70.0f);
                break;
            case 10:
                mace->MoveTo(x - 130.0f * attackDirection, y - 50.0f);
                break;              
            default:
                break;
            }
        }

        if (jumpCd.Up())
        {
            if (state == FK_LEAP)
            {
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
            attackDirection = direction == LEFT ? -1.0f : 1.0f;

            animation->Select(FK_LEAPING * direction);
            animation->Restart();

            JumpTo(J_MIDDLE);
        }

        if (jumpCd.Up() && !rageStarted)
        {
            rageStarted = true;
            attackRageCd.Restart();
            rageCd.Restart();
        }

        if (rageStarted)
        {

            if (attackRageCd.Up())
            {
                attackDirection = -attackDirection;
                direction = direction == LEFT ? LEFT : RIGHT;
                spawnedBarrels = false;
                attackRageCd.Restart();
            }

            animation->Select(FK_SLAMMING* direction);
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
                    Sprite* sp = new Sprite("Resources/WIP/rock.png");

                    Barrel* b1 = new Barrel(sp);
                    Barrel* b2 = new Barrel(sp);
                    //Barrel* b3 = new Barrel(sp);

                    TP2::scene->Add(b1, MOVING);
                    TP2::scene->Add(b2, MOVING);
                    //TP2::scene->Add(b3, MOVING);
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

    // IDLE
    if (state == FK_IDLE)
    {
        animation->Select(state);
        // create cooldown and draft one movement every 2 seconds if not doing one
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

    float timeJumping = state == FK_BLUDGEON ? 1.4f : 2.0f;
    float jumpingTo = jumpToLocation == PLAYER ? TP2::player->X() : window->CenterX();

    ySpeed = state == FK_BLUDGEON ? bludgeonSpeed : leapSpeed;
    xSpeed = (jumpingTo - x) / timeJumping;
}
