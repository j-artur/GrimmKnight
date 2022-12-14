#include "Attack.h"
#include "GrimmKnight.h"
#include "Spike.h"
#include "Util.h"

Attack::Attack(TileSet *tileSet, Player *player, HDirection dir, Direction atkDir)
{
    type = ATTACK;

    anim = new Animation(tileSet, 0.075f, false);

    uint seqLeft[2] = {0, 1};
    uint seqRight[2] = {2, 3};
    uint seqDownLeft[2] = {4, 5};
    uint seqDownRight[2] = {6, 7};
    uint seqUpLeft[2] = {8, 9};
    uint seqUpRight[2] = {10, 11};

    anim->Add(LEFT * H_LEFT, seqLeft, 2);
    anim->Add(LEFT * H_RIGHT, seqLeft, 2);
    anim->Add(RIGHT * H_LEFT, seqRight, 2);
    anim->Add(RIGHT * H_RIGHT, seqRight, 2);
    anim->Add(DOWN * H_LEFT, seqDownLeft, 2);
    anim->Add(DOWN * H_RIGHT, seqDownRight, 2);
    anim->Add(UP * H_LEFT, seqUpLeft, 2);
    anim->Add(UP * H_RIGHT, seqUpRight, 2);

    anim->Select(dir * atkDir);

    this->player = player;
    this->direction = atkDir;

    switch (atkDir)
    {
    case LEFT:
        BBox(new Rect(-60.0f, -40.0f, 40.0f, 40.0f));
        break;
    case RIGHT:
        BBox(new Rect(-40.0f, -40.0f, 60.0f, 40.0f));
        break;
    case UP:
        BBox(new Rect(-40.0f, -60.0f, 40.0f, 50.0f));
        break;
    case DOWN:
        BBox(new Rect(-40.0f, -50.0f, 40.0f, 60.0f));
        break;
    }
}

Attack::~Attack()
{
    delete anim;
}

void Attack::Update()
{
    switch (direction)
    {
    case UP:
        MoveTo(player->X(), player->Y() - 48.0f);
        break;
    case DOWN:
        MoveTo(player->X(), player->Y() + 48.0f);
        break;
    case LEFT:
        MoveTo(player->X() - 32.0f, player->Y());
        break;
    case RIGHT:
        MoveTo(player->X() + 32.0f, player->Y());
        break;
    }

    anim->NextFrame();
    if (anim->Inactive())
        GrimmKnight::scene->Delete();
}

void Attack::OnCollision(Object *other)
{
    if (find(objectsHit.begin(), objectsHit.end(), other) == objectsHit.end())
    {
        objectsHit.push_back(other);
        switch (other->Type())
        {
        case ENEMY: {
            Entity *enemy = (Entity *)other;
            if (enemy->TakeDamage(5, direction))
            {
                GrimmKnight::audio->Play(ENEMY_DAMAGE);
                GrimmKnight::player->AddMana();
                GrimmKnight::player->Knockback();
            }
            break;
        }
        case RADIANCE: {
            Entity *radiance = (Entity *)other;
            if (radiance->TakeDamage(5, direction))
            {
                GrimmKnight::audio->Play(ENEMY_DREAM_DAMAGE);
                GrimmKnight::player->AddMana();
                GrimmKnight::player->Knockback();
            }
            break;
        }
        case SPIKE: {
            GrimmKnight::audio->Play(PLAYER_SPIKE_ATTACK);
            GrimmKnight::player->Knockback();
            break;
        }
        case TOTEM: {
            Entity *totem = (Entity *)other;
            if (totem->TakeDamage(1, direction))
            {
                GrimmKnight::audio->Play(SOUL_TOTEM);
                GrimmKnight::player->Knockback();
                GrimmKnight::player->AddMana();
            }
            break;
        }
        case BALDUR: {
            GrimmKnight::audio->Play(PLAYER_SPIKE_ATTACK);
            GrimmKnight::player->Knockback();
        }
        }
    }
}
