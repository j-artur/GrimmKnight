#include "Radiance.h"
#include "Beam.h"
#include "Sword.h"
#include "TP2.h"

Radiance::Radiance()
{
    type = ENEMY;

    hp = 65;

    random_device rd;
    rng.seed(rd());
    move = uniform_int_distribution<int>(1, 3);
    angle = uniform_int_distribution<int>(0, 60);
    nextMove = 2;
    betweenAttacksCd.Restart();

    tileSet = new TileSet("Resources/WIP/Radiance.png", 516, 516, 2, 2);
    swordSprite = new Sprite("Resources/WIP/RadianceSword.png");
    animation = new Animation(tileSet, 0.2f, true);

    uint seqIdle[2] = { 0,1 };

    animation->Add(RD_IDLE, seqIdle, 2);

    animation->Select(RD_IDLE);

    //MoveTo(iX * 32.0f, iY * 32.0f);

    DraftAngle(RD_BEAM_A);

    Mixed *bb = new Mixed();

    head = new Rect(-48.0f, -36.0f, 48.0f, 36.0f);
    Rect *body = new Rect(-96.0f, -48.0f, 96.0f, 48.0f);
    Rect* lowBody = new Rect(-36.0f, -28.0f, 36.0f, 28.0f);

    bb->Insert(head);
    bb->Insert(body);
    bb->Insert(lowBody);

    head->MoveTo(bb->X(), bb->Y() - 140.0f);
    body->Translate(0, -56.0f);
    lowBody->MoveTo(bb->X(), bb->Y() + 26.0f);

    BBox(bb);
}

Radiance::~Radiance()
{
    delete tileSet;
    delete animation;
}

bool Radiance::TakeDamage(uint damage, Direction dir)
{
    if (hurtCd.Up())
    {
        hp -= damage;

        return true;

        hurtCd.Restart();
    }
    return false;
}

void Radiance::DraftAngle(RD_Attack attack)
{
    startingAngle = angle(rng);
    for (int i = 0; i < 6; i++)
    {
        direction[i] = Vector(i * 60.0f + startingAngle, attack);
    }
}

void Radiance::Update()
{

    if (window->KeyPress('L'))
    {
        state = RD_SWORD_BURST;
    }

    if (state == RD_BEAM_BURST)
    {
        DraftAngle(RD_BEAM_A);
        for (auto i : direction)
        {
            Beam* b = new Beam(i);
            b->MoveTo(head->X() + i.XComponent(), head->Y() - i.YComponent());
            TP2::scene->Add(b, STATIC);
        }
        
        state = RD_IDLE;
    }

    if (state == RD_SWORD_BURST)
    {
        DraftAngle(RD_SWORD_A);
        for (auto i : direction)
        {
            Sword* s = new Sword(i, swordSprite);
            s->MoveTo(head->X() + i.XComponent(), head->Y() - i.YComponent());
            TP2::scene->Add(s, MOVING);
        }

        state = RD_IDLE;
    }


    betweenAttacksCd.Add(gameTime);

    animation->NextFrame();
}

void Radiance::Draw()
{
    animation->Draw(round(x), round(y), LAYER_BOSS);
}

void Radiance::DraftMove()
{
    oldMove = nextMove;
    nextMove = move(rng);

    if (nextMove == oldMove)
        DraftMove();
}
