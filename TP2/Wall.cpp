#include "Wall.h"

Wall::Wall(int iX1, int iY1, int iWidth, int iHeight)
{
    this->type = WALL;

    // float width = abs(x2 - x1);
    // float height = abs(y2 - y1);
    float x1 = iX1 * 32.0f;
    float y1 = iY1 * 32.0f;
    float width = iWidth * 32.0f;
    float height = iHeight * 32.0f;

    float x2 = x1 + width;
    float y2 = y1 + height;

    MoveTo((x1 + x2) / 2.0f, (y1 + y2) / 2.0f);
    BBox(new Rect(-width / 2.0f, -height / 2.0f, width / 2.0f, height / 2.0f));
};

void Wall::Update()
{
}

void Wall::Draw()
{
}
