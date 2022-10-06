
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Animation.h"
#include "Object.h"
#include "Sprite.h"
#include "Util.h"
#include "Vector.h"
#include "Wall.h"

enum PlayerState
{
    IDLE_RIGHT,
    IDLE_LEFT,
    WALK_RIGHT,
    WALK_LEFT
};

class Player : public Object
{
  private:
    TileSet *tileSet;
    Sprite *light;
    Animation *animation;
    PlayerState state;

    float oldTop;
    float oldBottom;
    float oldLeft;
    float oldRight;

    bool jumping = false;
    bool standing = false;
    float jumpingAmount = 0.0f;
    bool dying = false;

    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    float maxXSpeed = 288.0f;
    float jumpingSpeed = -464.0f;
    float gravity = 768.0f;

  public:
    Player();
    ~Player();

    void Update();
    void Draw();
    void OnCollision(Object *other);
};

#endif
