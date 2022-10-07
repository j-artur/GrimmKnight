#include "ScreenTransition.h"
#include "TP2.h"
#include "Util.h"

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

bool ScreenTransition::Transitioning()
{
    return transitioning;
}

void ScreenTransition::Update()
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
        }

        switch (orientation)
        {
        case HORIZONTAL:
            if (positive)
            {
                scene->Apply([&](Object *obj)
                             {
                                 if (!transitioning && (obj->Type() == WALL_TOP || obj->Type() == WALL_BOTTOM || obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT))
                                 {
                                     Wall *wall = (Wall *)obj;
                                     float newX = wall->absX - window->Width();
                                     wall->MoveTo(newX, wall->absY);
                                     wall->absX = newX;
                                 }
                                 else
                                     obj->Translate(-window->Width() * time, 0.0f); });
                TP2::player->Translate(DISTANCE * time, 0.0f);
            }
            else
            {
                scene->Apply([&](Object *obj)
                             {
                                 if (!transitioning && (obj->Type() == WALL_TOP || obj->Type() == WALL_BOTTOM || obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT))
                                 {
                                     Wall *wall = (Wall *)obj;
                                     float newX = wall->absX + window->Width();
                                     wall->MoveTo(newX, wall->absY);
                                     wall->absX = newX;
                                 }
                                 else
                                     obj->Translate(window->Width() * time, 0.0f); });
                TP2::player->Translate(-DISTANCE * time, 0.0f);
            }
            break;
        case VERTICAL:
            if (positive)
            {
                scene->Apply([&](Object *obj)
                             {
                                 if (!transitioning && (obj->Type() == WALL_TOP || obj->Type() == WALL_BOTTOM || obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT))
                                 {
                                     Wall *wall = (Wall *)obj;
                                     float newY = wall->absY - window->Height();
                                     wall->MoveTo(wall->absX, newY);
                                     wall->absY = newY;
                                 }
                                 else
                                     obj->Translate(0.0f, -window->Height() * time); });
                TP2::player->Translate(0.0f, DISTANCE * time);
            }
            else
            {
                scene->Apply([&](Object *obj)
                             {
                                 if (!transitioning && (obj->Type() == WALL_TOP || obj->Type() == WALL_BOTTOM || obj->Type() == WALL_LEFT || obj->Type() == WALL_RIGHT))
                                 {
                                     Wall *wall = (Wall *)obj;
                                     float newY = wall->absY + window->Height();
                                     wall->MoveTo(wall->absX, newY);
                                     wall->absY = newY;
                                 }
                                 else
                                     obj->Translate(0.0f, window->Height() * time); });
                TP2::player->Translate(0.0f, -DISTANCE * time);
            }
            break;
        }
    }
}

void ScreenTransition::OnCollision(Object *other)
{
    if (other->Type() == PLAYER)
    {
        if (!transitioning)
        {
            transitioning = true;
            timer = 0.0f;

            switch (orientation)
            {
            case HORIZONTAL:
                positive = TP2::player->X() < X();
                break;
            case VERTICAL:
                positive = TP2::player->Y() < Y();
                break;
            }
        }
    }
}
