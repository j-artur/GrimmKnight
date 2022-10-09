#ifndef _SCREEN_TRANSITION_H_
#define _SCREEN_TRANSITION_H_

#include "Object.h"
#include "Scene.h"
#include "Util.h"
#include <unordered_map>

struct Coord
{
    float x;
    float y;
};

class ScreenTransition : public Object
{
  private:
    Scene *scene;
    std::unordered_map<Object *, Coord> positions;
    Orientation orientation;
    AttackDirection dir;
    bool positive;
    float timer;
    bool transitioning = false;

    static const float DURATION;
    static const float DISTANCE;

  public:
    ScreenTransition(Orientation orientation, Scene *scene);

    bool Transitioning();

    void Update();
    void Draw(){};
    void OnCollision(Object *other);
};

inline bool ScreenTransition::Transitioning()
{
    return transitioning;
}

#endif
