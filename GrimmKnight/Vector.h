#ifndef _PROGJOGOS_VECTOR_H_
#define _PROGJOGOS_VECTOR_H_

#include <cmath>

class Vector
{
  private:
    static const double PI;
    float angle;
    float magnitude;

  public:
    Vector();
    Vector(float ang, float mag);

    void RotateTo(float value);
    void ScaleTo(float value);
    void Rotate(float theta);
    void Scale(float factor);
    void Add(const Vector &v);

    float Angle() const;
    float Magnitude() const;
    float XComponent() const;
    float YComponent() const;
    float Radians() const;
};

inline void Vector::RotateTo(float value)
{
    angle = value;
}

inline void Vector::ScaleTo(float value)
{
    magnitude = value;
}

inline float Vector::Angle() const
{
    return angle;
}

inline float Vector::Magnitude() const
{
    return magnitude;
}

inline float Vector::XComponent() const
{
    return magnitude * cos(Radians());
}

inline float Vector::YComponent() const
{
    return magnitude * sin(Radians());
}

inline float Vector::Radians() const
{
    return float(angle * PI / 180.0);
}

#endif
