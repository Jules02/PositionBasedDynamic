#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle
{
    Vec2 pos;
    Vec2 expectedPos;
    Vec2 velocity;
    Vec2 appliedForces;
    float radius;
    float mass;
    bool isActivated;

    Particle(Vec2 _pos, Vec2 _velocity, float _radius, float _mass)
        : pos(_pos), expectedPos(_pos), velocity(_velocity), radius(_radius), mass(_mass), isActivated(false) {}
};

#endif // PARTICLE_H
