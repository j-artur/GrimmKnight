#ifndef _COOLDOWN_H_
#define _COOLDOWN_H_

class Cooldown
{
  private:
    float time;
    float duration;

  public:
    Cooldown(float duration, bool up = true);

    bool Up();
    bool Down();
    bool Over(float time);

    void Add(float dt);
    void Restart();
    void Leave(float time);
};

#endif
