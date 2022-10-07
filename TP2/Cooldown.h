#ifndef _COOLDOWN_H_
#define _COOLDOWN_H_

class Cooldown
{
  private:
    float time;
    float duration;

  public:
    Cooldown(float duration);

    void Add(float dt);
    bool Ready();
    bool Elapsed(float time);
    void Reset();
    void Left(float time);
};

#endif
