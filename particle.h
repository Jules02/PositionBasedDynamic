#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle
{
    Vec2 pos;
    Vec2 velocity;
    float radius;
    float mass;

    Particle(Vec2 _pos, Vec2 _velocity, float _radius, float _mass)
        : pos(_pos), velocity(_velocity), radius(_radius), mass(_mass) {}
};

#endif // PARTICLE_H
