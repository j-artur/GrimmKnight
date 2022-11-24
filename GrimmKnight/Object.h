#ifndef _PROGJOGOS_OBJECT_H_
#define _PROGJOGOS_OBJECT_H_

#include "Game.h"
#include "Geometry.h"
#include "Types.h"
#include "Window.h"

class Object
{
  private:
    float posX, posY, posZ;
    float scaleFactor;
    float rotationAngle;
    Geometry *bbox;

  protected:
    static Window *&window;
    static Game *&game;
    static float &gameTime;

    const float &x = posX;
    const float &y = posY;
    const float &z = posZ;

    const float &scale = scaleFactor;

    const float &rotation = rotationAngle;

    uint type;

  public:
    Object();

    virtual ~Object();
    virtual float X() const;
    virtual float Y() const;
    virtual float Z() const;
    virtual float Scale() const;
    virtual float Rotation() const;
    virtual uint Type() const;

    virtual void BBox(Geometry *bb);
    virtual Geometry *BBox() const;

    virtual void Translate(float dx, float dy, float dz = 0.0f);
    virtual void Scale(float factor);
    virtual void ScaleTo(float value);
    virtual void Rotate(float angle);
    virtual void RotateTo(float value);
    virtual void MoveTo(float px, float py, float pz);
    virtual void MoveTo(float px, float py);
    virtual void OnCollision(Object *obj);
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

inline float Object::X() const
{
    return posX;
}

inline float Object::Y() const
{
    return posY;
}

inline float Object::Z() const
{
    return posY;
}

inline float Object::Scale() const
{
    return scaleFactor;
}

inline float Object::Rotation() const
{
    return rotationAngle;
}

inline uint Object::Type() const
{
    return type;
}

inline Geometry *Object::BBox() const
{
    return bbox;
}

#endif
