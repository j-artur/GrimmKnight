#include "FalseKnight.h"
#include "Shockwave.h"
#include "TP2.h"

FalseKnight::FalseKnight()
{
    type = ENEMY;

    mace = new Mace();
    TP2::scene->Add(mace, MOVING);

    tileSet = new TileSet("Resources/WIP/FalseKnight.png", 303, 192, 5, 35);
    shockwaveTileSet = new TileSet("Resources/attack.png", 64, 64, 5, 10);
    animation = new Animation(tileSet, 0.2f, true);

    uint idle[5] = { 0,1,2,3,4 };
    uint prepSlam[6] = { 15,16,17,18,19,20 };
    uint attack[8] = { 21,22,23,24,25,26,27,28 };

    animation->Add(FK_IDLE, idle, 5);
    animation->Add(FK_PREP_SLAM, prepSlam, 6);
    animation->Add(FK_SLAMMING, attack, 8);

    animation->Select(state);
    BBox(new Rect(-80, -192 / 2, 150, 192 / 2));
}

FalseKnight::~FalseKnight()
{
    delete tileSet;
    delete animation;
}

void FalseKnight::Update()
{
    ySpeed += gravity * gameTime;
    Translate(0, ySpeed * gameTime);
    mace->MoveTo(x - 120.0f, y);


    // testing slamming
    if (window->KeyDown('K') && keyCtrl)
    {
        keyCtrl = false;
        slamCd.Reset();
        prepSlamCd.Reset();
        state = FK_SLAM;
    }
    else if (window->KeyUp('K'))
    {
        keyCtrl = true;
    }

    // missing left slam
    if (state == FK_SLAMMING)
    {
        uint oldAnimState = animation->Sequence();
        if (!prepSlamCd.Ready())
        {
            animation->Select(FK_PREP_SLAM);
            mace->MoveTo(x - 150.0f, y + 50.0f);
        }
        else
        {
            animation->Select(FK_SLAMMING);
            animation->Delay(0.1f);
            switch (animation->Frame())
            {
            case 0:
                mace->MoveTo(x - 50.0f, y - 140.0f);
                break;
            case 1:
                mace->MoveTo(x + 150.0f * directionMult, y - 50.0f);
                break;
            case 2:
                mace->MoveTo(x + 160.0f, y + 50.0f);
                if (!spawnedShockwave)
                {
                    Shockwave* shockwave = new Shockwave(RIGHT, shockwaveTileSet);
                    shockwave->MoveTo(mace->X(), mace->Y());
                    TP2::scene->Add(shockwave, MOVING);
                    spawnedShockwave = true;
                }
                break;
            case 3:
                mace->MoveTo(x, y - 140.0f);
                break;
            case 4:
                mace->MoveTo(x - 40.0f, y - 140.0f);
                break;
            case 5:
                mace->MoveTo(x - 60.0f, y - 110.0f);
                break;
            case 6:
                mace->MoveTo(x - 40.0f, y - 80.0f);
                break;
            case 7:
                mace->MoveTo(x - 120.0f, y);
                break;

            default:
                break;
            }
        }

        if (slamCd.Ready())
        {
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

    if (state == FK_IDLE)
    {
        animation->Select(state);
    }

    animation->NextFrame();    
}

void FalseKnight::Draw()
{
    animation->Draw(x,y, Layer::MIDDLE);
}

void FalseKnight::OnCollision(Object* other)
{
    switch (other->Type())
    {
    case WALL_TOP: {
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

        bool justEntered = self->Bottom() >= wall->Y();
        bool isInside = self->Left() != wallBBox->Right();

        if (justEntered && isInside)
        {
            MoveTo(x, other->Y() - self->bottom);
            ySpeed = 0.0f;
        }
        break;
    }
    case WALL_LEFT: {
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

        bool justEntered = self->Right() >= wall->X();
        bool isInside = self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->right, y);
        }
        break;
    }
    case WALL_RIGHT: {
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

        bool justEntered = self->Left() <= wall->X();
        bool isInside = self->Top() != wallBBox->Bottom();

        if (justEntered && isInside)
        {
            MoveTo(other->X() - self->left, y);
        }
        break;
    }
    }
}

