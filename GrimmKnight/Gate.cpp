#include "Gate.h"
#include "GrimmKnight.h"

Gate::Gate(Image *img, HDirection direction, int iX, int iY)
{
    sprite = new Sprite(img);
    this->direction = direction;

    float height = 128.0f;
    float y1 = iY * 32.0f;
    float y2 = y1 + height;

    float absX = iX * 32.0f + 16.0f;
    float absY = (y1 + y2) / 2.0f;

    if (direction == H_LEFT)
    {
        type = WALL_RIGHT;
        BBox(new Rect(16.0f, -height / 2.0f, 16.0f, height / 2.0f));
    }
    else
    {
        type = WALL_LEFT;
        BBox(new Rect(-16.0f, -height / 2.0f, -16.0f, height / 2.0f));
    }

    MoveTo(absX, absY);
}

Gate::~Gate()
{
    delete sprite;
}

void Gate::Open()
{
    if (opened)
        return;
    moving = true;
    opened = true;
    MoveTo(x, y - 96.0f);
}

void Gate::Close()
{
    if (!opened)
        return;
    moving = true;
    opened = false;
    MoveTo(x, y + 96.0f);
}

void Gate::Update()
{
}

void Gate::Draw()
{
    sprite->Draw(x, y, LAYER_GATE);
}
