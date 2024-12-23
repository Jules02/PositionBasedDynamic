#ifndef CONTEXT_H
#define CONTEXT_H

#include "particle.h"

class Context
{
public:
    Context();
    std::vector<Particle> circles;
    bool isGravityOn;

    void updatePhysicalSystem(float dt);
};

#endif // CONTEXT_H
