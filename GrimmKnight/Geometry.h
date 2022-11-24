#ifndef _PROGJOGOS_GEOMETRY_H_
#define _PROGJOGOS_GEOMETRY_H_

#include "Types.h"
#include <list>
using std::list;

enum GeometryTypes
{
    UNKNOWN_T,
    POINT_T,
    LINE_T,
    RECTANGLE_T,
    CIRCLE_T,
    POLYGON_T,
    MIXED_T
};

class Geometry
{
  protected:
    float x, y;
    float scale;
    float rotation;
    uint type;

  public:
    Geometry();
    virtual ~Geometry();

    virtual float X() const
    {
        return x;
    }

    virtual float Y() const
    {
        return y;
    }

    virtual float Scale() const
    {
        return scale;
    }

    virtual float Rotation() const
    {
        return rotation;
    }

    virtual uint Type() const
    {
        return type;
    }

    virtual void Translate(float dx, float dy)
    {
        x += dx;
        y += dy;
    }

    virtual void Scale(float factor)
    {
        scale *= factor;
    }

    virtual void ScaleTo(float value)
    {
        scale = value;
    }

    virtual void Rotate(float angle)
    {
        rotation += angle;
    }

    virtual void RotateTo(float value)
    {
        rotation = value;
    }

    virtual void MoveTo(float px, float py)
    {
        x = px;
        y = py;
    }
};

class Point : public Geometry
{
  public:
    Point();
    Point(float posX, float posY);
    Point(int posX, int posY);

    float Distance(const Point &p) const;
};

class Line : public Geometry
{
  public:
    Point a, b;

    Line();
    Line(float x1, float y1, float x2, float y2);
    Line(Point &pa, Point &pb);

    float Ax() const
    {
        return x + (a.X() * scale);
    }
    float Ay() const
    {
        return y + (a.Y() * scale);
    }
    float Bx() const
    {
        return x + (b.X() * scale);
    }
    float By() const
    {
        return y + (b.Y() * scale);
    }
};

class Rect : public Geometry
{
  public:
    float left;
    float top;
    float right;
    float bottom;

    Rect();
    Rect(float x1, float y1, float x2, float y2);
    Rect(Point &a, Point &b);

    float Left() const
    {
        return x + (left * scale);
    }
    float Top() const
    {
        return y + (top * scale);
    }
    float Right() const
    {
        return x + (right * scale);
    }
    float Bottom() const
    {
        return y + (bottom * scale);
    }
};

class Circle : public Geometry
{
  private:
    float radius;

  public:
    Circle();
    Circle(float r);

    float Radius() const
    {
        return radius * scale;
    }
    float CenterX() const
    {
        return x;
    }
    float CenterY() const
    {
        return y;
    }
};

class Poly : public Geometry
{
  public:
    uint vertexCount;
    Point *vertexList;

    Poly();
    Poly(Point *vList, uint vCount);
    Poly(const Poly &p);
    ~Poly();

    const Poly &operator=(const Poly &p);
};

class Mixed : public Geometry
{
  public:
    list<Geometry *> shapes;

    Mixed();
    ~Mixed();

    void Insert(Geometry *s);
    void Remove(Geometry *s);

    void Translate(float dx, float dy);
    void Scale(float factor);
    void ScaleTo(float value);
    void MoveTo(float px, float py);
};

#endif
