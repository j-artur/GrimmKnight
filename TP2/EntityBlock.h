#ifndef _ENTITY_BLOCK_H_
#define _ENTITY_BLOCK_H_

#include "Object.h"
#include "Util.h"

class EntityBlock : public Object
{
  public:
    float absX;
    float absY;
    void Update(){};
    void Draw(){};
};

class EntityBlockTop : public EntityBlock
{
  public:
    EntityBlockTop(int x, int y, int w);
};

class EntityBlockBottom : public EntityBlock
{
  public:
    EntityBlockBottom(int x, int y, int w);
};

class EntityBlockLeft : public EntityBlock
{
  public:
    EntityBlockLeft(int x, int y, int h);
};

class EntityBlockRight : public EntityBlock
{
  public:
    EntityBlockRight(int x, int y, int h);
};

#endif
