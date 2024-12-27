#include "context.h"
#include <stdio.h>

Context::Context() {}

void Context::addStaticContactConstraints() {
    activeConstraints.clear();

    // Check for contact between each collider/particle pair
    for (auto& collider : colliders) {
        for (Particle& particle : circles) {
            auto constraint = collider->checkContact(particle);
            if (constraint) {
                activeConstraints.push_back(*constraint);

                particle.isActivated = true;
            }
        }
    }
}

void Context::updatePhysicalSystem(float dt) {

    // Refer to Equations.pdf from the assignement for more details on the different steps

    for (Particle& circle: circles) {

        // 1. Update velocities
        this->applyExternalForce(dt, circle);
        circle.velocity = circle.velocity + (dt / circle.mass) * circle.appliedForces;
        // 2. Compute expected positions
        circle.expectedPos = circle.pos + dt * circle.velocity;

    }

    // 3.
    this->projectConstraints();

    for (Particle& circle: circles) {

        // 3bis. Resolve constraints
        this->updateExpectedPosition(dt, circle);
        circle.delta = -0.5*circle.delta;
        // 4. Update velocity and position
        this->updateVelocityAndPosition(dt, circle);
    }

}

void Context::applyExternalForce(float dt, Particle& particle) {
    // GRAVITY
    if (this->isGravityOn) {
        particle.appliedForces = 0.0001 * Vec2{0, static_cast<float>(-particle.mass*9.81)};
    }
}

void Context::updateExpectedPosition(float dt, Particle& particle) {
    particle.expectedPos = particle.expectedPos + particle.delta;
}

void Context::updateVelocityAndPosition(float dt, Particle& particle) {
    particle.velocity = 1/dt * (particle.expectedPos - particle.pos);
    particle.pos = particle.expectedPos;
}

void Context::enforceStaticGroundConstraint(const StaticConstraint& constraint) {
    //Vec2 q_c = constraint.particle->pos - dot(constraint.particle->pos - constraint.origin, constraint.normal) * constraint.normal;
    //float C = dot(constraint.particle->pos - q_c, constraint.normal) - constraint.particle->radius;
    //Vec2 delta = -C * constraint.normal;
    constraint.particle->delta = constraint.delta;
    constraint.particle->velocity = 0.90 * constraint.particle->velocity;
}


void Context::projectConstraints() {
    for (StaticConstraint& constraint: activeConstraints) {
        enforceStaticGroundConstraint(constraint);
    }
}


