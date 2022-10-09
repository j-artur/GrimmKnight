#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Game.h"

enum LevelId
{
    TITLESCREEN,
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
};

class Level : public Game
{
  protected:
    LevelId id;

  public:
    LevelId Id();

    virtual void EnterFrom(LevelId id) = 0;
};

inline LevelId Level::Id()
{
    return id;
}

#endif
