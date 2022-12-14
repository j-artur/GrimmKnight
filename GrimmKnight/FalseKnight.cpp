#include "FalseKnight.h"
#include "Barrel.h"
#include "GrimmKnight.h"
#include "Level2.h"
#include "Shockwave.h"
#include "Wall.h"

FalseKnight::FalseKnight(int iX, int iY)
{
    type = ENEMY;

    hp = headHealth;

    random_device rd;
    rng.seed(rd());
    move = uniform_int_distribution<int>(1, 3);
    nextMove = 2;
    betweenAttacksCd.Restart();

    tileSet = new TileSet("Resources/FalseKnight.png", 452, 260, 6, 24);
    headTileSet = new TileSet("Resources/FalseKnightHead.png", 76, 76, 2, 4);
    shockwaveTileSet = new TileSet("Resources/Shockwave.png", 132, 132, 2, 4);
    barrelSprite = new Sprite("Resources/Barrel.png");
    animation = new Animation(tileSet, 0.2f, true);

    uint idleRight[2] = {0, 1};
    uint idleLeft[2] = {12, 13};
    uint leapRight[2] = {2, 3};
    uint leapLeft[2] = {14, 15};
    uint prepSlamRight[1] = {4};
    uint prepSlamLeft[1] = {16};
    uint attackRight[2] = {6, 7};
    uint attackLeft[2] = {18, 19};
    uint bludgeonJumpRight[2] = {2, 8};
    uint bludgeonJumpLeft[2] = {14, 20};
    uint bludgeonAttackRight[1] = {9};
    uint bludgeonAttackLeft[1] = {21};
    uint stunRight[3] = {10, 11, 5};
    uint stunLeft[3] = {22, 23, 17};
    uint deadRight[1] = {5};
    uint deadLeft[1] = {17};
    uint inactive[1] = {23};

    animation->Add(FK_IDLE * H_LEFT, idleLeft, 2);
    animation->Add(FK_IDLE * H_RIGHT, idleRight, 2);
    animation->Add(FK_PREP_SLAM * H_LEFT, prepSlamLeft, 1);
    animation->Add(FK_PREP_SLAM * H_RIGHT, prepSlamRight, 1);
    animation->Add(FK_SLAMMING * H_LEFT, attackLeft, 2);
    animation->Add(FK_SLAMMING * H_RIGHT, attackRight, 2);
    animation->Add(FK_LEAPING * H_LEFT, leapLeft, 2);
    animation->Add(FK_LEAPING * H_RIGHT, leapRight, 2);
    animation->Add(FK_BLUDGEONING_J * H_RIGHT, bludgeonJumpRight, 2);
    animation->Add(FK_BLUDGEONING_J * H_LEFT, bludgeonJumpLeft, 2);
    animation->Add(FK_BLUDGEONING * H_RIGHT, bludgeonAttackRight, 1);
    animation->Add(FK_BLUDGEONING * H_LEFT, bludgeonAttackLeft, 1);
    animation->Add(FK_STUN * H_RIGHT, stunRight, 3);
    animation->Add(FK_STUN * H_LEFT, stunLeft, 3);
    animation->Add(FK_DEAD * H_RIGHT, deadRight, 1);
    animation->Add(FK_DEAD * H_LEFT, deadLeft, 1);
    animation->Add(FK_INACTIVE, inactive, 1);

    MoveTo(iX * 32.0f, iY * 32.0f);

    animation->Select(state * direction);

    head = new FalseKnightHead(headTileSet);

    bb = new Mixed();

    self = new Rect(-64.0f, -16.0f, 64.0f, 124.0f);
    hand = new Rect(-21.0f, -21.0f, 21.0f, 21.0f);
    horn = new Rect(-30.0f, -20.0f, 30.0f, 27.0f);
    hit = new Rect(-40.0f, -40.0f, 30.0f, 27.0f);
    mace = new Circle(35.0f);

    bb->Insert(self);
    bb->Insert(horn);
    bb->Insert(hand);
    bb->Insert(hit);
    bb->Insert(mace);

    if (GrimmKnight::fkDefeated)
    {
        state = FK_DEAD;
        animation->Select(FK_DEAD * H_LEFT);
    }

    BBox(bb);
}

FalseKnight::~FalseKnight()
{
    delete tileSet;
    delete headTileSet;
    GrimmKnight::scene->Delete(head, STATIC);
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
            GrimmKnight::audio->Play(SFK_HEAD_DAMAGE);
            head->TakeDamage();
            return true;
        }
        else
        {
            hurtCd.Restart();
            currentArmorHealth -= damage;
            GrimmKnight::audio->Play(SFK_ARMOR_DAMAGE);
            GrimmKnight::player->Knockback();
            return false;
        }
    }

    return false;
}

void FalseKnight::Activate()
{
    state = FK_IDLE;
    active = true;
    GrimmKnight::audio->Stop(CAVE_NOISES);
    GrimmKnight::audio->Play(SFK_THEME, true);
}

void FalseKnight::Update()
{
    direction = GrimmKnight::player->X() < x ? H_LEFT : H_RIGHT;
    directionMult = direction == H_LEFT ? -1.0f : 1.0f;

    mace->MoveTo(bb->X() - 145.0f * directionMult, bb->Y() + 40.0f);
    hand->MoveTo(bb->X() + 80.0f * directionMult, bb->Y() + 40.0f);
    horn->MoveTo(bb->X() + 2.0f, bb->Y() - 43.0f);
    hit->MoveTo(bb->X(), bb->Y() + 40.0f);

    if (window->KeyPress('J'))
    {
        currentArmorHealth = 0;
    }

    if (betweenAttacksCd.Up() && !isAttacking && state == FK_IDLE)
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
            attackRageCd.Restart();
            rageCd.Restart();
            jumpCd.Restart();
            state = FK_RAGE;
            break;
        default:
            break;
        }
    }
    if (state == FK_IDLE)
    {
        animation->Loop(true);
        animation->Delay(0.2f);
        animation->Select(state * direction);
    }
    if (state == FK_LEAP || state == FK_BLUDGEON)
    {
        if (!isJumping)
        {
            GrimmKnight::audio->Play(SFK_JUMP);

            attackDirection = direction;

            animation->Select(state == FK_LEAP ? FK_LEAPING * direction : FK_BLUDGEONING_J * direction);
            animation->Restart();

            JumpTo(J_PLAYER);
        }

        if (jumpCd.Up())
        {
            animation->Loop(true);
            animation->Delay(0.2f);

            if (state == FK_LEAP)
            {
                GrimmKnight::audio->Play(SFK_LAND);
                slamCd.Restart();
                prepSlamCd.Restart();
                isJumping = false;
                state = FK_SLAM;
            }
            else
            {
                if (strikeCtrl)
                {
                    strikeCtrl = false;
                    animation->Select(FK_BLUDGEONING * attackDirection);
                    GrimmKnight::audio->Play(SFK_STRIKE_GROUND);
                    bludgeonAttackCd.Restart();
                }
                else if (bludgeonAttackCd.Up())
                {
                    isJumping = false;
                    state = FK_IDLE;
                }
                else
                {
                    directionMult = attackDirection == H_LEFT ? -1.0f : 1.0f;
                    mace->MoveTo(bb->X() + 120.0f * directionMult, bb->Y() + 100.0f);
                    hit->Translate(100.0f * directionMult, -20.0f);
                    hand->Translate(0.0f, 10.0f);
                }
                isAttacking = false;

                betweenAttacksCd.Restart();
                DraftMove();
            }
        }

        bludgeonAttackCd.Add(gameTime);
        jumpCd.Add(gameTime);
    }
    if (state == FK_SLAM)
    {
        uint oldAnimState = animation->Sequence();
        if (!prepSlamCd.Up())
        {
            animation->Select(FK_PREP_SLAM * direction);
            horn->MoveTo(bb->X() - 50.0f * directionMult, bb->Y() - 43.0f);
        }
        else
        {
            animation->Select(FK_SLAMMING * direction);
            animation->Delay(0.1f);
            animation->Loop(false);
            mace->MoveTo(bb->X() - 40.0f * directionMult, bb->Y() - 80.0f);
            hit->Translate(80.0f * directionMult, 0);

            if (slamCd.Over(1.4f))
            {
                mace->MoveTo(bb->X() + 100.0f * directionMult, bb->Y() + 80.0f);
                if (!spawnedShockwave)
                {
                    GrimmKnight::audio->Play(SFK_STRIKE_GROUND);
                    Shockwave *shockwave = new Shockwave(mace, direction, shockwaveTileSet);
                    GrimmKnight::scene->Add(shockwave, MOVING);
                    spawnedShockwave = true;
                }
            }
        }

        if (slamCd.Up())
        {
            isAttacking = false;
            betweenAttacksCd.Restart();
            DraftMove();

            spawnedShockwave = false;
            animation->Delay(0.2f);
            animation->Loop(true);
            state = FK_IDLE;
        }

        if (oldAnimState != animation->Sequence())
        {
            animation->Restart();
        }

        slamCd.Add(gameTime);
        prepSlamCd.Add(gameTime);
    }
    if (state == FK_RAGE)
    {
        if (!isJumping)
        {
            GrimmKnight::audio->Play(SFK_JUMP);

            animation->Select(FK_LEAPING * direction);

            JumpTo(J_MIDDLE);
        }

        if (jumpCd.Up() && !rageStarted)
        {
            animation->Loop(true);
            animation->Delay(0.2f);
            GrimmKnight::audio->Play(SFK_LAND);
            GrimmKnight::audio->Play(SFK_VOICE_RAGE);
            attackRageCd.Restart();
            rageCd.Restart();
            attackDirection = direction;
            rageStarted = true;
        }

        if (rageStarted)
        {
            if (attackRageCd.Up())
            {
                attackDirection = attackDirection == H_LEFT ? H_RIGHT : H_LEFT;
                spawnedBarrels = false;
                animRestarted = false;
                voiceCtrl = true;
                strikeCtrl = true;
                attackRageCd.Restart();
            }
            else
            {
                if (!animRestarted)
                {
                    animation->Select(FK_SLAMMING * attackDirection);
                    animation->Loop(false);
                    animation->Restart();

                    animRestarted = true;
                }

                switch (animation->Frame())
                {
                case 0: {
                    mace->MoveTo(bb->X() - 40.0f * directionMult, bb->Y() - 80.0f);
                    hit->MoveTo(bb->X() + 80.0f * directionMult, bb->Y());

                    if (voiceCtrl)
                    {
                        GrimmKnight::audio->Play(SFK_SWING);

                        voiceCtrl = false;
                    }
                    break;
                }
                case 1: {
                    mace->MoveTo(bb->X() + 100.0f * directionMult, bb->Y() + 80.0f);
                    if (!spawnedBarrels)
                    {
                        spawnedBarrels = true;

                        GrimmKnight::scene->Add(new Barrel(barrelSprite), MOVING);
                    }
                    if (strikeCtrl)
                    {
                        GrimmKnight::audio->Play(SFK_STRIKE_GROUND);

                        strikeCtrl = false;
                    }
                    break;
                }

                default:
                    break;
                }
            }
        }

        if (rageCd.Up())
        {
            isAttacking = false;
            betweenAttacksCd.Restart();
            DraftMove();

            animation->Delay(0.2f);
            spawnedBarrels = false;
            rageStarted = false;
            state = FK_IDLE;
        }

        attackRageCd.Add(gameTime);
        rageCd.Add(gameTime);
        jumpCd.Add(gameTime);
    }
    if (state == FK_STUN)
    {
        isAttacking = false;
        mace->MoveTo(x, y);
        if (!isStunned)
        {
            GrimmKnight::audio->Play(SFK_STUN);
            headOutCd.Restart();
            stunCd.Restart();

            stunDirection = direction;

            animation->Loop(false);
            animation->Restart();
            animation->Select(FK_STUN * stunDirection);

            ySpeed = 0;

            isStunned = true;
        }

        if (headOutCd.Up() && isStunned)
        {
            if (!headOut)
            {
                animation->Select(FK_STUN * stunDirection);
                GrimmKnight::scene->Add(head, STATIC);
                headOut = true;
            }

            head->MoveTo(x + 73.0f * (stunDirection == H_LEFT ? -1.0f : 1.0f), y + 80.0f);
            head->setDirection(stunDirection);

            if (hp <= 0)
            {
                if (!canKill)
                {
                    GrimmKnight::scene->Remove(head, STATIC);

                    currentArmorHealth = armorHealth;
                    hp = headHealth;
                    canKill = true;
                }

                isStunned = false;
                headOut = false;
                state = FK_IDLE;
            }
            else if (stunCd.Up())
            {
                GrimmKnight::scene->Remove(head, STATIC);

                currentArmorHealth = armorHealth;
                isStunned = false;
                headOut = false;
                state = FK_IDLE;
            }
        }
        stunCd.Add(gameTime);
        headOutCd.Add(gameTime);
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
            state = FK_DEAD;
            Level2::rightGate->Open();
            GrimmKnight::scene->Remove(head, STATIC);
            if (voice2Ctrl)
            {
                animation->Select(FK_DEAD * stunDirection);

                GrimmKnight::audio->Stop(SFK_THEME);
                GrimmKnight::audio->Play(SFK_DEFEAT);
                GrimmKnight::fkDefeated = true;
                voice2Ctrl = false;
            }
            horn->MoveTo(bb->X(), bb->Y() + 40.0f);
            hand->MoveTo(bb->X(), bb->Y() + 40.0f);
            mace->MoveTo(bb->X(), bb->Y() + 40.0f);
            hit->MoveTo(bb->X(), bb->Y() + 40.0f);
        }
    }
    ySpeed += gravity * gameTime;
    Translate(xSpeed * gameTime, ySpeed * gameTime);

    betweenAttacksCd.Add(gameTime);
    hurtCd.Add(gameTime);

    animation->NextFrame();
}

void FalseKnight::Draw()
{
    if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        animation->Draw(round(x), round(y), Alive() ? LAYER_BOSS : LAYER_ENEMY, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else
        animation->Draw(round(x), round(y), Alive() ? LAYER_BOSS : LAYER_ENEMY);
}

void FalseKnight::OnCollision(Object *other)
{
    if (other->BBox()->Type() == RECTANGLE_T && Collision(self, (Rect *)other->BBox()))
    {
        switch (other->Type())
        {
        case ENTITY_BLOCK_BOTTOM:
        case WALL_TOP: {
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
        case ENTITY_BLOCK_RIGHT:
        case WALL_LEFT: {
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
        case ENTITY_BLOCK_LEFT:
        case WALL_RIGHT: {
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

    switch (other->Type())
    {
    case BARREL: {
        Barrel *barrel = (Barrel *)other;
        GrimmKnight::audio->Play(SFK_BARREL_DEATH);
        GrimmKnight::scene->Delete(barrel, MOVING);
        TakeDamage(5, LEFT);
        break;
    }
    }
}

void FalseKnight::JumpTo(FK_JumpTo jumpToLocation)
{
    jumpCd.Restart();
    isJumping = true;
    animation->Loop(false);
    animation->Delay(0.1f);
    if (voiceCtrl)
    {
        voiceCtrl = false;

        if (state == FK_BLUDGEON)
            GrimmKnight::audio->Play(SFK_SWING);
        if (canKill)
            GrimmKnight::audio->Play(SFK_VOICE_ATTACK);
    }

    float jumpingTo = jumpToLocation == PLAYER ? GrimmKnight::player->X() : window->CenterX();

    ySpeed = jumpSpeed;
    xSpeed = (jumpingTo - x) / 2;
}

void FalseKnight::DraftMove()
{
    oldMove = nextMove;
    nextMove = move(rng);

    if (oldMove == FK_RAGE && nextMove == oldMove)
        DraftMove();
}
