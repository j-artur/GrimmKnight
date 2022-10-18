#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Object.h"
#include "Sprite.h"
#include <functional>

class Button : public Object
{
  private:
    Sprite *sprite = nullptr;
    std::function<void(void)> action;
    bool hover = false;
    float hoverTime = 0.0f;
    float hoverTimeMax = 0.1f;

  public:
    Button(Sprite *sprite, std::function<void(void)> action);

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
