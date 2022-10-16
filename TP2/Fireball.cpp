#include "Fireball.h"
#include "TP2.h"

Fireball::Fireball(Player *player, HDirection direction)
{
    type = FIREBALL;

    this->direction = direction;

    ts = new TileSet("Resources/Fireball.png", 256, 64, 2, 12);
    anim = new Animation(ts, 0.2f, true);

    uint spawnRightSeq[3] = {0, 2, 4};
    uint spawnLeftSeq[3] = {6, 8, 10};
    uint flyingRightSeq[3] = {1, 3, 5};
    uint flyingLeftSeq[3] = {7, 9, 11};

    anim->Add(SPAWNING * H_LEFT, spawnLeftSeq, 3);
    anim->Add(SPAWNING * H_RIGHT, spawnRightSeq, 3);
    anim->Add(FLYING * H_LEFT, flyingLeftSeq, 3);
    anim->Add(FLYING * H_RIGHT, flyingRightSeq, 3);

    anim->Select(SPAWNING * direction);

    if (direction == H_LEFT)
    {
        speed = -speed;
        distance = -distance;
    }

    BBox(new Rect(-60.0f, -28.0f, 60.0f, 28.0f));

    MoveTo(player->X() + distance, player->Y());
}

// ---------------------------------------------------------------------------------

Fireball::~Fireball()
{
    delete ts;
    delete anim;
}

// ---------------------------------------------------------------------------------

void Fireball::Update()
{
    Translate(speed * gameTime, 0);

    if (spawnCd.Up())
    {
        anim->Select(FLYING * direction);
    }
    else
        spawnCd.Add(gameTime);

    anim->NextFrame();
}

// ---------------------------------------------------------------------------------

void Fireball::OnCollision(Object *obj)
{
    if (find(objectsHit.begin(), objectsHit.end(), obj) == objectsHit.end())
    {
        objectsHit.push_back(obj);
        switch (obj->Type())
        {
        case WALL_LEFT:
        case WALL_RIGHT:
        case SCREEN_TRANSITION:
        case LEVEL_TRANSITION:
            // TODO: Add fireball death animation
            TP2::scene->Delete(this, MOVING);
            break;
        case ENEMY: {
            Entity *enemy = (Entity *)obj;
            if (enemy->Alive())
                enemy->TakeDamage(15, Dir());
            break;
        }
        }
    }
}
