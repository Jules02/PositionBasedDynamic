#ifndef CONTEXT_H
#define CONTEXT_H

#include "object.h"
#include "collider.h"
#include "constraint.h"

class Context
{
public:
    Context();

    std::vector<Object> objects;

    bool isGravityOn;

    std::vector<std::unique_ptr<Collider>> colliders;
    std::vector<std::unique_ptr<Constraint>> activeConstraints;
    inline void addCollider(std::unique_ptr<Collider> collider) { colliders.push_back(std::move(collider)); }
    inline void addConstraint(std::unique_ptr<Constraint> constraint) { activeConstraints.push_back(std::move(constraint)); }

    void updatePhysicalSystem(float dt);

private:
    void applyExternalForce(float dt);
    void dampVelocities();
    void updateExpectedPosition(float dt);
    void updateNeighbors();
    void addFluidConstraints();
    void addDynamicContactConstraints();
    void addStaticContactConstraints();
    void enforceConstraint(const Constraint&, Particle&);
    void projectConstraints();
    void updateVelocityAndPosition(float dt);
    void applyFriction(float dt);
    void deleteContactConstraints();
};

#endif // CONTEXT_H
