#include "FalseKnight.h"
#include "TP2.h"

FalseKnight::FalseKnight()
{
    type = BOSS;

    tileSet = new TileSet("Resources/idle.png", 149, 96, 1, 5);
    animation = new Animation(tileSet, 0.20f, true);
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
                state = STILL;
        }
        break;
    }
    case WALL_BOTTOM: {
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

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
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

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
        Rect* self = (Rect*)BBox();
        Wall* wall = (Wall*)other;
        Rect* wallBBox = (Rect*)wall->BBox();

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

