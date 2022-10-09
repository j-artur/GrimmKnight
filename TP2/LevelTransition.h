#ifndef _LEVEL_TRANSITION_H_
#define _LEVEL_TRANSITION_H_

#include "Level.h"
#include "Object.h"
#include "Scene.h"
#include "Util.h"

class LevelTransition : public Object
{
  private:
    AttackDirection dir;
    float timer;
    bool transitioning = false;
    bool done = false;

  public:
    static const float DURATION;
    static const float DISTANCE;

    LevelTransition(AttackDirection dir);

    bool Transitioning();
    bool Done();

    void Update();
    void Draw(){};
    void OnCollision(Object *other);
};

inline bool LevelTransition::Transitioning()
{
    return transitioning;
}

inline bool LevelTransition::Done()
{
    return done;
}

#endif
