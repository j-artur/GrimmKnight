#include "Radiance.h"
#include "Beam.h"
#include "GrimmKnight.h"
#include "Orb.h"
#include "Sword.h"

Radiance::Radiance()
{
    type = RADIANCE;

    hp = 350;

    random_device rd;
    rng.seed(rd());
    move = uniform_int_distribution<int>(RD_TELEPORT, RD_ORB);
    angle = uniform_int_distribution<int>(0, 60);
    nextMove = RD_SWORD_RAIN;
    betweenAttacksCd.Restart();

    tileSet = new TileSet("Resources/Radiance.png", 516, 516, 2, 2);
    swordSprite = new Sprite("Resources/RadianceSword.png");
    orbSprite = new Sprite("Resources/Orb.png");
    beamTileSet = new TileSet("Resources/Beam.png", 2, 1);

    animation = new Animation(tileSet, 0.2f, true);

    uint seqIdle[2] = {0, 1};

    animation->Add(RD_IDLE, seqIdle, 2);

    animation->Select(RD_IDLE);

    // MoveTo(iX * 32.0f, iY * 32.0f);

    DraftAngle(RD_BEAM_A);
    hDirection = 0.0f, vDirection = 270.0f;

    Mixed *bb = new Mixed();

    Rect *head = new Rect(-48.0f, -36.0f, 48.0f, 36.0f);
    Rect *body = new Rect(-96.0f, -48.0f, 96.0f, 48.0f);
    Rect *lowBody = new Rect(-36.0f, -28.0f, 36.0f, 28.0f);

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
    delete swordSprite;
    delete orbSprite;
    delete animation;
}

bool Radiance::TakeDamage(uint damage, Direction dir)
{
    if (hurtCd.Down())
        return false;

    hurtCd.Restart();
    hp -= damage;
    if (hp < 0)
        hp = 0;

    return true;
}

void Radiance::DraftAngle(RD_Attack attack)
{
    startingAngle = angle(rng);
    for (int i = 0; i < 6; i++)
    {
        directions[i] = Vector(i * 60.0f + startingAngle, attack);
    }
}

void Radiance::DraftGaps(uint n)
{
    uniform_int_distribution<int> rnd = uniform_int_distribution<int>(0, n - 1);
    for (int i = 1; i < 3; i++)
    {
        gaps[i] = rnd(rng);
    }

    uniform_int_distribution<int> gap0 = uniform_int_distribution<int>(4, 5);
    gaps[0] = gap0(rng);
}

HDirection Radiance::DraftDirection()
{
    uniform_int_distribution<int> rnd = uniform_int_distribution<int>(0, 1);
    return rnd(rng) == 0 ? H_LEFT : H_RIGHT;
}

void Radiance::DraftSpawn()
{
    // x 256 - 1024
    // y 256 - 640
    uniform_int_distribution<int> rndX = uniform_int_distribution<int>(256, 1024);
    uniform_int_distribution<int> rndY = uniform_int_distribution<int>(256, 640);

    spawnX = rndX(rng);
    spawnY = rndY(rng);
}

void Radiance::DraftTeleport()
{
    uniform_int_distribution<int> rnd = uniform_int_distribution<int>(1, 3);
    uint r;
    switch (rnd(rng))
    {
    case 1:
        r = 256.0f;
        break;
    case 2:
        r = 640.0f;
        break;
    case 3:
        r = 1024.0f;
        break;
    default:
        break;
    }

    if (r == x)
        return DraftTeleport();

    spawnX = r;
}

void Radiance::DraftMove()
{
    oldMove = nextMove;
    nextMove = move(rng);

    if (nextMove == oldMove)
        return DraftMove();

    if (nextMove == RD_BEAM_WALL)
        projectileXSpawn = (DraftDirection() == H_LEFT ? window->Width() : 0.0f);

    count = 0;
}

void Radiance::Update()
{
    if (state == RD_IDLE && betweenAttacksCd.Up())
    {
        betweenAttacksCd.Restart();
        DraftMove();

        switch (nextMove)
        {
        case RD_TELEPORT:
            preTeleport.Restart();
            posTeleport.Restart();
            ctrl = true;
            state = RD_TELEPORT;
            break;
        case RD_SWORD_BURST:
            state = RD_SWORD_BURST;
            break;
        case RD_SWORD_WALL:
            state = RD_SWORD_WALL;
            break;
        case RD_SWORD_RAIN:
            state = RD_SWORD_RAIN;
            break;
        case RD_BEAM_WALL:
            spawningBeamCd.Restart();
            state = RD_BEAM_WALL;
            break;
        case RD_BEAM_BURST:
            state = RD_BEAM_BURST;
            break;
        case RD_ORB:
            state = RD_ORB;
            break;
        default:
            break;
        }
    }

    // TELEPORT
    if (state == RD_TELEPORT)
    {
        if (ctrl && preTeleport.Up())
        {
            ctrl = false;
            DraftTeleport();
            MoveTo(spawnX, y);
            GrimmKnight::audio->Play(SRD_TELEPORT, false);
        }
        else if (posTeleport.Up())
            state = RD_IDLE;

        preTeleport.Add(gameTime);
        posTeleport.Add(gameTime);
    }

    // BEAM BURST
    if (state == RD_BEAM_BURST)
    {
        DraftAngle(RD_BEAM_A);
        GrimmKnight::audio->Play(SRD_LASER_PREPARE);
        for (auto i : directions)
        {
            Beam *b = new Beam(beamTileSet, i.Angle());
            b->MoveTo(x + i.XComponent(), y - 90.0f - i.YComponent());
            GrimmKnight::scene->Add(b, STATIC);
        }

        state = RD_IDLE;
    }

    // BEAM WALL
    if (state == RD_BEAM_WALL)
    {
        if (spawningBeamCd.Up() && count < 20)
        {
            GrimmKnight::audio->Play(SRD_LASER_PREPARE);
            Beam *b = new Beam(beamTileSet, vDirection);
            b->MoveTo(projectileXSpawn + (projectileXSpawn == 0.0f ? 40.0f : -40.0f) * count, 0.0f);
            GrimmKnight::scene->Add(b, STATIC);
            count++;
            spawningBeamCd.Restart();
        }

        if (count >= 20)
            state = RD_IDLE;

        spawningBeamCd.Add(gameTime);
    }

    // SWORD BURST
    if (state == RD_SWORD_BURST)
    {
        DraftAngle(RD_SWORD_A);
        GrimmKnight::audio->Play(SRD_SWORD_BURST);
        for (auto i : directions)
        {
            Sword *s = new Sword(swordSprite, i.Angle());
            s->MoveTo(x + i.XComponent(), y - 90.0f - i.YComponent());
            GrimmKnight::scene->Add(s, MOVING);
        }

        state = RD_IDLE;
    }

    // SWORD RAIN
    if (state == RD_SWORD_RAIN)
    {
        DraftGaps(17);

        GrimmKnight::audio->Play(SRD_SWORD_WALL);

        for (int i = 0; i < 17; i++)
        {
            if (i != gaps[0] && i != gaps[1] && i != gaps[2])
            {
                Sword *s = new Sword(swordSprite, vDirection);
                s->MoveTo(64.0f + 72 * i, 0.0f);
                GrimmKnight::scene->Add(s, MOVING);
            }
        }

        state = RD_IDLE;
    }

    // SWORD WALL
    if (state == RD_SWORD_WALL)
    {
        if (betweenAttacksCd.Up() && count < 4)
        {
            betweenAttacksCd.Restart();
            betweenAttacksCd.Leave(1.5f);

            if (DraftDirection() == H_LEFT)
            {
                hDirection = 180.0f;
                projectileXSpawn = window->Width();
            }
            else
            {
                hDirection = 0.0f;
                projectileXSpawn = 0.0f;
            }

            DraftGaps(6);

            GrimmKnight::audio->Play(SRD_SWORD_WALL);

            for (int i = 0; i < 6; i++)
            {
                if (i != gaps[0] && i != gaps[1] && i != gaps[2])
                {
                    Sword *s = new Sword(swordSprite, hDirection);
                    s->MoveTo(projectileXSpawn, 202.0f + 96.0f * i);
                    GrimmKnight::scene->Add(s, MOVING);
                }
            }

            count++;
        }
        if (count >= 4)
            state = RD_IDLE;
    }

    // ORB
    if (state == RD_ORB)
    {
        if (betweenAttacksCd.Up() && count < 5)
        {
            betweenAttacksCd.Restart();
            DraftSpawn();
            Orb *orb = new Orb(orbSprite);
            orb->MoveTo(spawnX, spawnY);
            GrimmKnight::scene->Add(orb, MOVING);
            GrimmKnight::audio->Play(SRD_ORB);

            count++;
        }

        if (count >= 5)
            state = RD_IDLE;
    }

    hurtCd.Add(gameTime);
    betweenAttacksCd.Add(gameTime);

    animation->NextFrame();
}

void Radiance::Draw()
{
    if (hurtCd.Down())
    {
        float f = 10.0f - 9.0f * hurtCd.Ratio();
        animation->Draw(round(x), round(y), LAYER_RADIANCE, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else if (preTeleport.Down())
    {
        float f = 1.0f + 9.0f * preTeleport.Ratio();
        animation->Draw(round(x), round(y), LAYER_RADIANCE, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else if (posTeleport.Down())
    {
        float f = 10.0f - 9.0f * posTeleport.Ratio();
        animation->Draw(round(x), round(y), LAYER_RADIANCE, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else
        animation->Draw(round(x), round(y), LAYER_RADIANCE);
}
