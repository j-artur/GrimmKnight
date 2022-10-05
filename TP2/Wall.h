#ifndef _WALL_H_
#define _WALL_H_

#include "Object.h"
#include "Util.h"

class Wall : public Object
{
  public:
    void Update(){};
    void Draw(){};
};

class TopWall : public Wall
{
  public:
    TopWall(int x, int y, int width);
};

class BottomWall : public Wall
{
  public:
    BottomWall(int x, int y, int width);
};

class LeftWall : public Wall
{
  public:
    LeftWall(int x, int y, int height);
};

class RightWall : public Wall
{
  public:
    RightWall(int x, int y, int height);
};

#endif
