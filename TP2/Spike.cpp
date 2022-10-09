#include "Spike.h"
#include "Util.h"

Spike::Spike(int iX, int iY, int iWidth, int iHeight, AttackDirection dir)
{
    type = SPIKE;

    this->dir = dir;

    float x1 = iX * 32.0f;
    float width = iWidth * 32.0f;
    float x2 = x1 + width;

    float y1 = iY * 32.0f;
    float height = iHeight * 32.0f;
    float y2 = y1 + height;

    absX = (x1 + x2) / 2.0f;
    absY = (y1 + y2) / 2.0f;

    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));

    MoveTo(absX, absY);
}
