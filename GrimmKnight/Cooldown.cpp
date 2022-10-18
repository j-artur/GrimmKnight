#include "Cooldown.h"

Cooldown::Cooldown(float duration, bool up)
{
    this->duration = duration;
    time = up ? duration : 0.0f;
}

void Cooldown::Leave(float time)
{
    if (duration - this->time > time)
        this->time = duration - time;
}
