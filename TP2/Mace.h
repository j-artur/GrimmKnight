#ifndef _MACE_H_
#define _MACE_H_

#include "Object.h"
#include "Types.h"

class Mace : public Object
{
public:
    Mace();
    ~Mace();

    void Update();
    void Draw();
};

inline void Mace::Draw()
{
}

#endif
