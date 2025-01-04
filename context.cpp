#include "context.h"
#include <stdio.h>

Context::Context() {}

void Context::updatePhysicalSystem(float dt) {

    // Refer to Equations.pdf from the assignement for more details on the different steps
    this->applyExternalForce(dt);             // (5)
    this->dampVelocities(dt);                 // (6)
    this->updateExpectedPosition(dt);         // (7)
    this->addDynamicContactConstraints(dt);   // (8)
    this->addStaticContactConstraints(dt);    // (8)
    this->projectConstraints();               // (9-11)
    this->updateVelocityAndPosition(dt);      // (12-15)
    this->applyFriction(dt);                  // (16)

}

void Context::applyExternalForce(float dt) {
    for (Particle& particle: circles) {
        // GRAVITY
        if (this->isGravityOn) {
            particle.appliedForces = 0.0003 * Vec2{0, static_cast<float>(-particle.mass*9.81)};
        }

        particle.velocity = particle.velocity + (dt / particle.mass) * particle.appliedForces;
    }
}

void Context::dampVelocities(float dt) {
    for (Particle& particle: circles) particle.velocity = 0.97*particle.velocity;
}

void Context::updateExpectedPosition(float dt) {
    for (Particle& particle: circles) particle.expectedPos = particle.pos + dt * particle.velocity;
}

void Context::addDynamicContactConstraints(float dt) {

}

void Context::addStaticContactConstraints(float dt) {

    // GENERATE CONSTRAINTS

    activeConstraints.clear();

    // Check for contact between each collider/particle pair
    for (auto& collider : colliders) {
        // TEMPORARY !

        auto* planCollider = dynamic_cast<PlanCollider*>(collider.get());
        if (!planCollider) continue;


        for (Particle& particle : circles) {
            StaticConstraint constraint(*planCollider, &particle);

            if (constraint.isSatisfied()) {

                activeConstraints.push_back(constraint);

                particle.isActivated = true;
            }
        }
    }
}

void Context::enforceStaticGroundConstraint(const StaticConstraint& constraint, Particle& particle) {
    particle.expectedPos = particle.expectedPos + constraint.delta;
}


void Context::projectConstraints() {
    for (StaticConstraint& constraint: activeConstraints) {
        enforceStaticGroundConstraint(constraint, *constraint.particle);
    }
}


void Context::updateVelocityAndPosition(float dt) {
    for (Particle& particle: circles) {
        particle.velocity = 1/dt * (particle.expectedPos - particle.pos);
        particle.pos = particle.expectedPos;
    }
}

void Context::applyFriction(float dt) {

}

