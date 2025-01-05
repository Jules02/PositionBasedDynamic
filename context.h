#ifndef CONTEXT_H
#define CONTEXT_H

#include "collider.h"
#include "constraint.h"

class Context
{
public:
    Context();
    std::vector<Particle> circles;
    bool isGravityOn;

    std::vector<std::unique_ptr<Collider>> colliders;
    std::vector<std::unique_ptr<Constraint>> activeConstraints;

    inline void addCollider(std::unique_ptr<Collider> collider) { colliders.push_back(std::move(collider)); }
    inline void addConstraint(std::unique_ptr<Constraint> constraint) { activeConstraints.push_back(std::move(constraint)); }

    void updatePhysicalSystem(float dt);

private:
    void applyExternalForce(float dt);
    void dampVelocities(float dt);
    void updateExpectedPosition(float dt);
    void addDynamicContactConstraints(float dt);
    void addStaticContactConstraints(float dt);
    void enforceConstraint(const Constraint&, Particle&);
    void projectConstraints();
    void updateVelocityAndPosition(float dt);
    void applyFriction(float dt);
    void deleteContactConstraints();
};

#endif // CONTEXT_H
