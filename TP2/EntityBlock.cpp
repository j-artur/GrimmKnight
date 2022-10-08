#include "EntityBlock.h"

EntityBlockTop::EntityBlockTop(int iX, int iY, int iWidth)
{
    type = ENTITY_BLOCK_TOP;

    float x1 = iX * 32.0f;
    float width = iWidth * 32.0f;
    float x2 = x1 + width;

    absX = (x1 + x2) / 2.0f;
    absY = iY * 32.0f;

    BBox(new Rect(-width / 2.0f, 0.0f, width / 2.0f, 0.0f));

    MoveTo(absX, absY);
}

EntityBlockBottom::EntityBlockBottom(int iX, int iY, int iWidth)
{
    type = ENTITY_BLOCK_BOTTOM;

    float x1 = iX * 32.0f;
    float width = iWidth * 32.0f;
    float x2 = x1 + width;

    absX = (x1 + x2) / 2.0f;
    absY = (iY + 1) * 32.0f;

    BBox(new Rect(-width / 2.0f, 0.0f, width / 2.0f, 0.0f));

    MoveTo(absX, absY);
}

EntityBlockLeft::EntityBlockLeft(int iX, int iY, int iHeight)
{
    type = ENTITY_BLOCK_LEFT;

    float y1 = iY * 32.0f;
    float height = iHeight * 32.0f;
    float y2 = y1 + height;

    absX = iX * 32.0f;
    absY = (y1 + y2) / 2.0f;

    BBox(new Rect(0.0f, -height / 2.0f, 0.0f, height / 2.0f));

    MoveTo(absX, absY);
}

EntityBlockRight::EntityBlockRight(int iX, int iY, int iHeight)
{
    type = ENTITY_BLOCK_RIGHT;

    float y1 = iY * 32.0f;
    float height = iHeight * 32.0f;
    float y2 = y1 + height;

    absX = (iX + 1) * 32.0f;
    absY = (y1 + y2) / 2.0f;

    BBox(new Rect(0.0f, -height / 2.0f, 0.0f, height / 2.0f));

    MoveTo(absX, absY);
}
