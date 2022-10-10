#include "Cursor.h"
#include "Util.h"

Cursor::Cursor()
{
    type = CURSOR;

    BBox(new Point());
}

void Cursor::Update()
{
    MoveTo(window->MouseX(), window->MouseY());
}
