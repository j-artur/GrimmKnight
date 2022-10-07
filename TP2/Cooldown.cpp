#include "Cooldown.h"

Cooldown::Cooldown(float duration)
{
    this->duration = duration;
    time = 0.0f;
}

void Cooldown::Add(float dt)
{
    time += dt;
}

bool Cooldown::Ready()
{
    return time >= duration;
}

bool Cooldown::Elapsed(float time)
{
    return this->time >= time;
}

void Cooldown::Reset()
{
    time = 0.0f;
}

void Cooldown::Left(float time)
{
    if (duration - this->time > time)
        this->time = duration - time;
}