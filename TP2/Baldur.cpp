#include "Baldur.h"

Baldur::Baldur(int iX, int iY)
{
    type = BALDUR;

    tileSet = new TileSet("Resources/Baldur.png", 132, 132, 5, 5);
    animation = new Animation(tileSet, 0.3f, true);

    uint seqIdle[2] = {0, 1};
    uint seqBlock[1] = {2};
    uint seqDead[2] = {3, 4};

    animation->Add(BALDUR_IDLE, seqIdle, 2);
    animation->Add(BALDUR_BLOCK, seqBlock, 1);
    animation->Add(BALDUR_DEAD, seqDead, 2);

    BBox(new Rect(-66.0f, -70.0f, 66.0f, 62.0f));
    MoveTo(iX * 32.0f, iY * 32.0f + 4.0f);

    hp = 1;
    animation->Select(BALDUR_IDLE);
    actionArea = new ActionArea(-128.0f, -128.0f, 256.0f, 128.0f);
    actionArea->MoveTo(x, y);
    TP2::scene->Add(actionArea, MOVING);
    state = BALDUR_IDLE;

    rightWall = new RightWall(iX + 1, iY - 2, 4);
    TP2::scene->Add(rightWall, STATIC);
    topWall = new TopWall(iX - 2, iY - 2, 4);
    TP2::scene->Add(topWall, STATIC);
}

Baldur::~Baldur()
{
    delete animation;
    delete tileSet;
}

bool Baldur::TakeDamage(uint damage, Direction dir)
{
    return false;
}

void Baldur::Update()
{
    switch (state)
    {
    case BALDUR_IDLE:
        soundCtrl = true;
        if (actionArea->IsPlayerInside())
        {
            if (soundCtrl)
                TP2::audio->Play(BALDUR_BLOCK_SOUND);
            state = BALDUR_BLOCK;
            animation->Select(BALDUR_BLOCK);
        }
        break;
    case BALDUR_BLOCK:
        if (!actionArea->IsPlayerInside())
            state = BALDUR_IDLE;
        break;
    case BALDUR_DEAD:
        if (deathCd.Up())
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

    deathCd.Add(gameTime);
    hurtCd.Add(gameTime);

    animation->Select(state);
    animation->NextFrame();
}

void Baldur::Draw()
{
    if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        animation->Draw(x, y, LAYER_ENEMY, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else
        animation->Draw(x, y, LAYER_ENEMY);
}

void Baldur::OnCollision(Object *other)
{
    if (other->Type() == FIREBALL && state == BALDUR_IDLE)
    {
        TP2::baldurKilled = true;
        hp = 0;
        hurtCd.Restart();
        state = BALDUR_DEAD;
        TP2::scene->Delete(actionArea, MOVING);
        TP2::scene->Delete(rightWall, STATIC);
        TP2::scene->Delete(topWall, STATIC);
        deathCd.Restart();
    }
}
