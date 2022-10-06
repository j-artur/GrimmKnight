#include "Fireball.h"
#include "TP2.h"

Fireball::Fireball(Player* player, Facing direction)
{
    ts = new TileSet("Resources/attack.png", 64, 64, 5, 10);
    anim = new Animation(ts, 0.1f, true);

    int width = ts->TileWidth();
    int height = ts->TileHeight();

    speed = 800.0f;
    distance = 50.0f;

    if (!direction) {
        speed = -speed;
        distance = -distance;
    }

    MoveTo(player->X() + distance, player->Y());

    fireballTimer.Start();

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
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
    anim->NextFrame();
    Translate(speed * gameTime, 0);


    if (fireballTimer.Elapsed(2.0f))
        TP2::scene->Delete();
}