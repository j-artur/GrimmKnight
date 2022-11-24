#include "Geometry.h"
#include <cmath>

Geometry::Geometry()
{
    x = y = 0.0f;
    scale = 1.0f;
    rotation = 0.0f;
    type = UNKNOWN_T;
}

Geometry::~Geometry()
{
}

Point::Point()
{
    type = POINT_T;
}

Point::Point(float posX, float posY)
{
    MoveTo(posX, posY);
    type = POINT_T;
}

Point::Point(int posX, int posY)
{
    MoveTo(float(posX), float(posY));
    type = POINT_T;
}

float Point::Distance(const Point &p) const
{
    float deltaX = abs(p.X() - x);
    float deltaY = abs(p.Y() - y);

    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

Line::Line()
{
    a = Point();
    b = Point();
    type = LINE_T;
}

Line::Line(float x1, float y1, float x2, float y2)
{
    a = Point(x1, y1);
    b = Point(x2, y2);
    type = LINE_T;
}

Line::Line(Point &pa, Point &pb)
{
    a = pa;
    b = pb;
    type = LINE_T;
}

Rect::Rect()
{
    left = right = 0.0f;
    top = bottom = 0.0f;
    type = RECTANGLE_T;
}

Rect::Rect(float x1, float y1, float x2, float y2)
{
    left = x1;
    top = y1;
    right = x2;
    bottom = y2;
    type = RECTANGLE_T;
}

Rect::Rect(Point &a, Point &b)
{
    left = a.X();
    top = a.Y();
    right = b.X();
    bottom = b.Y();
    type = RECTANGLE_T;
}

Circle::Circle()
{
    radius = 0;
    type = CIRCLE_T;
}

Circle::Circle(float r)
{
    radius = r;
    type = CIRCLE_T;
}

Poly::Poly()
{
    vertexCount = 0;
    vertexList = nullptr;
    type = POLYGON_T;
}

Poly::Poly(Point *vList, uint vCount)
{
    vertexCount = vCount;

    vertexList = new Point[vCount];

    for (uint i = 0; i < vCount; ++i)
        vertexList[i].MoveTo(vList[i].X(), vList[i].Y());

    type = POLYGON_T;
}

Poly::Poly(const Poly &p)
{
    vertexCount = p.vertexCount;

    vertexList = new Point[vertexCount];

    for (uint i = 0; i < vertexCount; ++i)
        vertexList[i].MoveTo(p.vertexList[i].X(), p.vertexList[i].Y());

    type = POLYGON_T;
}

const Poly &Poly::operator=(const Poly &p)
{
    if (vertexList)
        delete[] vertexList;

    vertexCount = p.vertexCount;

    vertexList = new Point[vertexCount];

    for (uint i = 0; i < vertexCount; ++i)
        vertexList[i].MoveTo(p.vertexList[i].X(), p.vertexList[i].Y());

    type = POLYGON_T;

    return *this;
}

Poly::~Poly()
{
    if (vertexList)
        delete[] vertexList;
}

Mixed::Mixed()
{
    type = MIXED_T;
}

Mixed::~Mixed()
{
    for (auto i : shapes)
        delete i;
}

void Mixed::Insert(Geometry *s)
{
    shapes.push_back(s);
}

void Mixed::Remove(Geometry *s)
{
    auto i = find(shapes.begin(), shapes.end(), s);
    shapes.erase(i);
}

void Mixed::Translate(float dx, float dy)
{
    x += dx;
    y += dy;

    for (auto i : shapes)
        i->Translate(dx, dy);
}

void Mixed::Scale(float factor)
{
    scale *= factor;

    for (auto i : shapes)
        i->Scale(factor);
}

void Mixed::ScaleTo(float value)
{
    scale = value;

    for (auto i : shapes)
        i->Scale(value);
}

void Mixed::MoveTo(float px, float py)
{
    for (auto i : shapes)
    {
        float deltaX = i->X() - x;
        float deltaY = i->Y() - y;
        i->MoveTo(px + deltaX, py + deltaY);
    }

    x = px;
    y = py;
}
