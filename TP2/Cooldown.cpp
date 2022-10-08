#include "Cooldown.h"

Cooldown::Cooldown(float duration, bool up)
{
    this->duration = duration;
    time = up ? duration : 0.0f;
}

void Cooldown::Add(float dt)
{
    time += dt;
}

bool Cooldown::Up()
{
    return time >= duration;
}

bool Cooldown::Down()
{
    return time < duration;
}

bool Cooldown::Over(float time)
{
    return this->time >= time;
}

void Cooldown::Restart()
{
    time = 0.0f;
}

void Cooldown::Leave(float time)
{
    if (duration - this->time > time)
        this->time = duration - time;
}
