#ifndef STATICCONSTRAINT_H
#define STATICCONSTRAINT_H

#include "particle.h"

class StaticConstraint
{
public:
    Vec2 normal;
    Vec2 origin;
    Particle* particle;
};

#endif // STATICCONSTRAINT_H
