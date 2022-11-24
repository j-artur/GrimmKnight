#include "Object.h"
#include "Engine.h"

Window *&Object::window = Engine::window;
Game *&Object::game = Engine::game;
float &Object::gameTime = Engine::frameTime;

Object::Object()
{
    posX = posY = 0.0f;
    posZ = 0.5f;
    scaleFactor = 1.0f;
    rotationAngle = 0.0f;
    type = 0;
    bbox = nullptr;
}

Object::~Object()
{
    if (bbox)
        delete bbox;
}

void Object::BBox(Geometry *bb)
{
    if (bbox)
        delete bbox;

    bbox = bb;
    bbox->MoveTo(x, y);
    bbox->ScaleTo(scaleFactor);
    bbox->RotateTo(rotationAngle);
}

void Object::Translate(float dx, float dy, float dz)
{
    posX += dx;
    posY += dy;
    posZ += dz;

    if (bbox)
        bbox->Translate(dx, dy);
}

void Object::Scale(float factor)
{
    scaleFactor *= factor;

    if (bbox)
        bbox->Scale(factor);
}

void Object::ScaleTo(float value)
{
    scaleFactor = value;
    if (bbox)
        bbox->ScaleTo(value);
}

void Object::Rotate(float angle)
{
    rotationAngle += angle;

    if (bbox)
        bbox->Rotate(angle);
}

void Object::RotateTo(float value)
{
    rotationAngle = value;

    if (bbox)
        bbox->RotateTo(value);
}

void Object::MoveTo(float px, float py, float pz)
{
    posX = px;
    posY = py;
    posZ = pz;

    if (bbox)
        bbox->MoveTo(px, py);
}

void Object::MoveTo(float px, float py)
{
    posX = px;
    posY = py;

    if (bbox)
        bbox->MoveTo(px, py);
}

void Object::OnCollision(Object *obj)
{
}
