#include "LevelTransition.h"
#include "TP2.h"

const float LevelTransition::DURATION = 1.0f;
const float LevelTransition::DISTANCE = 80.0f;

LevelTransition::LevelTransition(AttackDirection dir)
{
    this->dir = dir;
    switch (dir)
    {
    case ATK_UP:
    case ATK_DOWN:
        BBox(new Rect(-64.0f, -32.0f, 64.0f, 32.0f));
        break;
    case ATK_LEFT:
    case ATK_RIGHT:
        BBox(new Rect(-32.0f, -64.0f, 32.0f, 64.0f));
        break;
    }
}

void LevelTransition::Update()
{
    if (transitioning)
    {
        timer += gameTime;

        float time = gameTime;

        if (timer >= DURATION)
        {
            transitioning = false;
            time = gameTime - (timer - DURATION);
            timer = 0.0f;
            done = true;
        }

        switch (dir)
        {
        case ATK_UP:
            TP2::player->Translate(0.0f, -DISTANCE * time);
            break;
        case ATK_DOWN:
            TP2::player->Translate(0.0f, DISTANCE * time);
            break;
        case ATK_LEFT:
            TP2::player->Translate(-DISTANCE * time, 0.0f);
            break;
        case ATK_RIGHT:
            TP2::player->Translate(DISTANCE * time, 0.0f);
            break;
        }
    }
}

void LevelTransition::OnCollision(Object *other)
{
    if (other->Type() == PLAYER)
    {
        if (!done && !transitioning)
        {
            transitioning = true;
            timer = 0.0f;
            TP2::StartTransition();
        }
    }
}
