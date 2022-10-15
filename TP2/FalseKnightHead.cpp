#include "FalseKnightHead.h"
#include "TP2.h"

FalseKnightHead::FalseKnightHead(TileSet* ts)
{
	this->tileSet = ts;
	animation = new Animation(tileSet, 0.1f, false);

	uint seqRight[2] = { 0,1 };
	uint seqLeft[2] = { 2,3 };

	animation->Add(H_RIGHT, seqRight, 2);
	animation->Add(H_LEFT, seqLeft, 2);
}

FalseKnightHead::~FalseKnightHead()
{
	delete animation;
}

void FalseKnightHead::TakeDamage()
{
	animation->Restart();
}

void FalseKnightHead::setDirection(HDirection d)
{
	direction = d;
}

void FalseKnightHead::Update()
{
	animation->Select(direction);
	animation->NextFrame();
}

void FalseKnightHead::Draw()
{
	animation->Draw(round(x), round(y), LAYER_BOSS_ATTACK);
}