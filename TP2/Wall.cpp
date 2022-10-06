#include "Wall.h"

TopWall::TopWall(int iX, int iY, int iWidth)
{
    type = WALL_TOP;

    float x1 = iX * 32.0f;
    float width = iWidth * 32.0f;
    float x2 = x1 + width;

    float y = iY * 32.0f;

    BBox(new Rect(-width / 2.0f, 0.0f, width / 2.0f, 0.0f));

    MoveTo((x1 + x2) / 2.0f, y);
}

BottomWall::BottomWall(int iX, int iY, int iWidth)
{
    type = WALL_BOTTOM;

    float x1 = iX * 32.0f;
    float width = iWidth * 32.0f;
    float x2 = x1 + width;

    float y = (iY + 1) * 32.0f;

    BBox(new Rect(-width / 2.0f, 0.0f, width / 2.0f, 0.0f));

    MoveTo((x1 + x2) / 2.0f, y);
}

LeftWall::LeftWall(int iX, int iY, int iHeight)
{
    type = WALL_LEFT;

    float y1 = iY * 32.0f;
    float height = iHeight * 32.0f;
    float y2 = y1 + height;

    float x = iX * 32.0f;

    BBox(new Rect(0.0f, -height / 2.0f, 0.0f, height / 2.0f));

    MoveTo(x, (y1 + y2) / 2.0f);
}

RightWall::RightWall(int iX, int iY, int iHeight)
{
    type = WALL_RIGHT;

    float y1 = iY * 32.0f;
    float height = iHeight * 32.0f;
    float y2 = y1 + height;

    float x = (iX + 1) * 32.0f;

    BBox(new Rect(0.0f, -height / 2.0f, 0.0f, height / 2.0f));

    MoveTo(x, (y1 + y2) / 2.0f);
}
