#include "Cursor.h"
#include "Util.h"

Cursor::Cursor()
{
    type = CURSOR;

    sprite = new Sprite("Resources/Cursor.png");

    BBox(new Point());
}

Cursor::~Cursor()
{
    delete sprite;
}

void Cursor::Update()
{
    MoveTo(window->MouseX(), window->MouseY());
}

void Cursor::Draw()
{
    sprite->Draw(X() + 10.0f, Y() + 10.0f, LAYER_CURSOR);
}
