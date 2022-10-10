#include "ScreenTransition.h"
#include "TP2.h"
#include "Util.h"
#include "Wall.h"

const float ScreenTransition::DURATION = 1.0f;
const float ScreenTransition::DISTANCE = 128.0f;

ScreenTransition::ScreenTransition(Orientation orientation, Scene *scene)
{
    this->orientation = orientation;
    this->scene = scene;
    switch (orientation)
    {
    case HORIZONTAL:
        BBox(new Rect(-32.0f, -64.0f, 32.0f, 64.0f));
        break;

    case VERTICAL:
        BBox(new Rect(-64.0f, -32.0f, 64.0f, 32.0f));
        break;
    }
}

void ScreenTransition::Update()
{
    if (!transitioning)
        return;

    timer += gameTime;
    float time = gameTime;
    if (timer >= DURATION)
    {
        transitioning = false;
        time = gameTime - (timer - DURATION);
        timer = 0.0f;
    }

    if (transitioning)
        switch (dir)
        {
        case RIGHT:
            scene->Apply([&](Object *obj) { obj->Translate(-window->Width() * time, 0.0f); });
            TP2::player->Translate(DISTANCE * time, 0.0f);
            break;
        case LEFT:
            scene->Apply([&](Object *obj) { obj->Translate(window->Width() * time, 0.0f); });
            TP2::player->Translate(-DISTANCE * time, 0.0f);
            break;
        case DOWN:
            scene->Apply([&](Object *obj) { obj->Translate(0.0f, -window->Height() * time); });
            TP2::player->Translate(0.0f, DISTANCE * time);
            break;
        case UP:
            scene->Apply([&](Object *obj) { obj->Translate(0.0f, window->Height() * time); });
            TP2::player->Translate(0.0f, -DISTANCE * time);
            break;
        }
    else
    {
        switch (dir)
        {
        case RIGHT:
            scene->Apply([&](Object *obj) { obj->MoveTo(positions[obj].x - window->Width(), positions[obj].y); });
            TP2::player->Translate(DISTANCE * DURATION, 0.0f);
            break;
        case LEFT:
            scene->Apply([&](Object *obj) { obj->MoveTo(positions[obj].x + window->Width(), positions[obj].y); });
            TP2::player->Translate(-DISTANCE * DURATION, 0.0f);
            break;
        case DOWN:
            scene->Apply([&](Object *obj) { obj->MoveTo(positions[obj].x, positions[obj].y - window->Height()); });
            TP2::player->Translate(0.0f, DISTANCE * DURATION);
            break;
        case UP:
            scene->Apply([&](Object *obj) { obj->MoveTo(positions[obj].x, positions[obj].y + window->Height()); });
            TP2::player->Translate(0.0f, -DISTANCE * DURATION);
            TP2::player->AtkDir(DOWN);
            TP2::player->Knockback();
            break;
        }
        positions.clear();
    }
}

void ScreenTransition::OnCollision(Object *other)
{
    if (other->Type() == PLAYER)
    {
        if (!transitioning)
        {
            scene->Apply([&](Object *obj) { positions[obj] = {obj->X(), obj->Y()}; });
            transitioning = true;
            timer = 0.0f;

            if (orientation == HORIZONTAL)
                dir = TP2::player->X() < X() ? RIGHT : LEFT;
            else
                dir = TP2::player->Y() < Y() ? DOWN : UP;
        }
    }
}
