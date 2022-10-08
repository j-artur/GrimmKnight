#ifndef _WALL_H_
#define _WALL_H_

#include "Object.h"
#include "Scene.h"
#include "Util.h"
#include <vector>

class Wall : public Object
{
  public:
    float absX;
    float absY;
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

void AddWalls(Scene *scene, int x, int y, int width, int height);

#endif
