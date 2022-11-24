#ifndef _PROGJOGOS_SCENE_H_
#define _PROGJOGOS_SCENE_H_

#include "Geometry.h"
#include <functional>
#include <list>
using std::function;
using std::list;
using std::pair;
class Object;

enum ObjectGroup
{
    STATIC,
    MOVING
};

using ObjectPair = pair<Object *, Object *>;
using ObjectDel = pair<Object *, int>;

class Scene
{
  private:
    list<Object *> statics;
    list<Object *> moving;

    list<ObjectPair> collisions;
    list<ObjectDel> toDelete;

    list<Object *>::iterator its;
    list<Object *>::iterator itm;
    list<Object *>::iterator it;

    int processing = STATIC;

    bool Collision(Point *p, Point *q);
    bool Collision(Point *p, Rect *r);
    bool Collision(Point *p, Circle *c);

    bool Collision(Rect *ra, Rect *rb);
    bool Collision(Rect *r, Point *p);
    bool Collision(Rect *r, Circle *c);

    bool Collision(Circle *ca, Circle *cb);
    bool Collision(Circle *c, Point *p);
    bool Collision(Circle *c, Rect *r);

    bool Collision(Mixed *m, Geometry *s);
    bool Collision(Geometry *s, Mixed *m);

    void ProcessDeleted();

  public:
    Scene();
    ~Scene();

    void Add(Object *obj, int type);
    void Remove(Object *obj, int type);
    void Remove();
    void Delete(Object *obj, int type);
    void Delete();
    uint Size();

    void Begin();
    Object *Next();

    void Update();
    void Draw();
    void DrawBBox();

    bool Collision(Object *oa, Object *ob);
    void CollisionDetection();

    void Apply(function<void(Object *)> fn);
};

inline bool Scene::Collision(Rect *r, Point *p)
{
    return Collision(p, r);
}

inline bool Scene::Collision(Circle *c, Point *p)
{
    return Collision(p, c);
}

inline bool Scene::Collision(Circle *c, Rect *r)
{
    return Collision(r, c);
}

inline bool Scene::Collision(Geometry *s, Mixed *m)
{
    return Collision(m, s);
}

#endif
