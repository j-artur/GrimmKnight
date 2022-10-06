#ifndef _COOLDOWN_H_
#define _COOLDOWN_H_

class Cooldown
{
  private:
    float time;
    float duration;

  public:
    Cooldown(float duration);

    float Duration();
    void Add(float dt);
    bool Ready();
    void Reset();
    void Left(float time);
};

#endif
