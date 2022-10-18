#include "FalseKnightHead.h"
#include "GrimmKnight.h"
#include <sstream>

FalseKnightHead::FalseKnightHead(TileSet *ts)
{
    this->tileSet = ts;
    animation = new Animation(tileSet, 0.25f, false);

    uint seqRight[2] = {0, 1};
    uint seqLeft[2] = {2, 3};

    animation->Add(H_RIGHT, seqRight, 2);
    animation->Add(H_LEFT, seqLeft, 2);
}

FalseKnightHead::~FalseKnightHead()
{
    delete animation;
}

void FalseKnightHead::TakeDamage()
{
    hurtCd.Restart();
    animation->Restart();
}

void FalseKnightHead::setDirection(HDirection d)
{
    direction = d;
}

void FalseKnightHead::Update()
{
    hurtCd.Add(gameTime);
    animation->Select(direction);
    animation->NextFrame();
}

void FalseKnightHead::Draw()
{
    if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_BOSS_ATTACK, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else
        animation->Draw(round(x), round(y), LAYER_BOSS_ATTACK);
}
