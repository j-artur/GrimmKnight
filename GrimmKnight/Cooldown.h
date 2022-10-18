#ifndef _COOLDOWN_H_
#define _COOLDOWN_H_

class Cooldown
{
  private:
    float time;
    float duration;

  public:
    Cooldown(float duration, bool up = true);

    float Time();
    float Duration();
    float Ratio();

    bool Up();
    bool Down();
    bool Over(float time);

    void Add(float dt);
    void Restart();
    void Leave(float time);
};

inline float Cooldown::Time()
{
    return time;
}

inline float Cooldown::Duration()
{
    return duration;
}

inline float Cooldown::Ratio()
{
    return time / duration;
}

inline bool Cooldown::Up()
{
    return time >= duration;
}

inline bool Cooldown::Down()
{
    return time < duration;
}

inline bool Cooldown::Over(float time)
{
    return this->time >= time;
}

inline void Cooldown::Add(float dt)
{
    time += dt;
}

inline void Cooldown::Restart()
{
    time = 0.0f;
}

#endif
