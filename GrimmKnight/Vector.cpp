#include "Vector.h"

const double Vector::PI = 3.1415926535;

Vector::Vector()
{
    angle = 0;
    magnitude = 0;
}

Vector::Vector(float ang, float mag)
{
    angle = ang;
    magnitude = mag;
}

void Vector::Rotate(float theta)
{
    angle -= theta;

    while (angle < 0 || angle >= 360)
        angle += (angle >= 360 ? -360.0f : 360.0f);
}

void Vector::Scale(float factor)
{
    magnitude *= factor;
}

void Vector::Add(const Vector &v)
{
    float rx, ry;

    rx = XComponent() + v.XComponent();
    ry = YComponent() + v.YComponent();

    magnitude = sqrt(pow(rx, 2.0f) + pow(ry, 2.0f));

    if (rx > 0)
    {
        if (ry >= 0)
        {
            angle = atan(ry / rx);

            angle = float((180.0 * angle) / PI);
        }
        else
        {
            angle = atan(ry / rx);

            angle = float((180.0 * angle) / PI) + 360.0f;
        }
    }
    else if (rx < 0)
    {
        angle = atan(ry / rx);

        angle = float((180.0 * angle) / PI) + 180.0f;
    }
    else
    {
        if (ry > 0)
            angle = 90.0f;
        else if (ry < 0)
            angle = 270.0f;
        else
            angle = v.angle;
    }
}
