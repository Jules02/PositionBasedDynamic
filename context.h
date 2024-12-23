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

private:
    void applyExternalForce(float dt, Particle& particle);
    void updateExpectedPosition(float dt, Particle& particle);
    void updateVelocityAndPosition(float dt, Particle& particle);
    //void addDynamicContactConstraints(float dt);
    //void addStaticContactConstraints(float dt);
    //void projectConstraints();
    //void applyFriction(float dt);
    //void deleteContactConstraints();
};

#endif // CONTEXT_H
