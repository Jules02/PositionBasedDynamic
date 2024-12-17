#ifndef CIRCLE_H
#define CIRCLE_H

#include "Vec2.h"

struct Circle
{
    Vec2 pos;
    float radius;

    Circle(Vec2 _pos, float _radius)
        : pos(_pos), radius(_radius) {}
};

#endif // CIRCLE_H
