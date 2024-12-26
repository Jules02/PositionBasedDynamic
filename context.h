#ifndef CONTEXT_H
#define CONTEXT_H

#include "collider.h"
#include "staticconstraint.h"

class Context
{
public:
    Context();
    std::vector<Particle> circles;
    bool isGravityOn;

    std::vector<std::unique_ptr<Collider>> colliders;
    std::vector<StaticConstraint> activeConstraints;

    inline void addCollider(std::unique_ptr<Collider> collider) { colliders.push_back(std::move(collider)); }
    void addStaticContactConstraints();
    void updatePhysicalSystem(float dt);

private:
    void applyExternalForce(float dt, Particle& particle);
    void updateExpectedPosition(float dt, Particle& particle);
    void updateVelocityAndPosition(float dt, Particle& particle);

    //void addDynamicContactConstraints(float dt);

    //void projectConstraints();
    //void applyFriction(float dt);
    //void deleteContactConstraints();
};

#endif // CONTEXT_H
