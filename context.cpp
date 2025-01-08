#include "context.h"
#include <stdio.h>

Context::Context() {}

void Context::updatePhysicalSystem(float dt) {

    // Refer to Equations.pdf from the assignement for more details on the different steps
    this->applyExternalForce(dt);             // (5)
    this->dampVelocities(dt);                 // (6)
    this->updateExpectedPosition(dt);         // (7)
    this->updateNeighbors(dt);
    this->activeConstraints.clear();
    this->addFluidConstraints(dt);
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
    for (Particle& particle: circles) particle.velocity = 0.96*particle.velocity;
}

void Context::updateExpectedPosition(float dt) {
    for (Particle& particle: circles) particle.expectedPos = particle.pos + dt * particle.velocity;
}

void Context::updateNeighbors(float dt) {
    for (Particle& particle_i : circles) {
        for (Particle& particle_j : circles) {
            if (length(particle_j.pos - particle_i.pos) < 50) {
                particle_i.neighbors.push_back(&particle_j);
            }
        }
    }
}

void Context::addFluidConstraints(float dt) {
    for (Particle& particle_i : circles) {
        auto constraint = std::make_unique<FluidConstraint>(&particle_i);
        if (constraint->isSatisfied()) {
            addConstraint(std::move(constraint));
        }
    }
}

void Context::addDynamicContactConstraints(float dt) {

    for (Particle& particle_i : circles) {
        for (const Particle& particle_j : circles) {
            if (&particle_i == &particle_j) continue;
            /*
            auto constraint = std::make_unique<DynamicConstraint>(particle_j, &particle_i);
            if (constraint->isSatisfied()) {

                addConstraint(std::move(constraint));

                particle_i.isActivated = true;
            }


            auto link = std::make_unique<DynamicLink>(particle_j, &particle_i);
            if (link->isSatisfied()) {
                addConstraint(std::move(link));
            }*/
        }
    }
}

void Context::addStaticContactConstraints(float dt) {

    // GENERATE CONSTRAINTS

    // Check for contact between each collider/particle pair
    for (auto& collider : colliders) {

        for (Particle& particle : circles) {
            auto constraint = std::make_unique<StaticConstraint>(*collider.get(), &particle);

            if (constraint->isSatisfied()) {

                addConstraint(std::move(constraint));

                particle.isActivated = true;
            }
        }
    }
}

void Context::enforceConstraint(const Constraint& constraint, Particle& particle) {
    particle.expectedPos = particle.expectedPos + constraint.getDelta();
}


void Context::projectConstraints() {
    for (const std::unique_ptr<Constraint>& constraint : activeConstraints) {
        enforceConstraint(*constraint, *constraint->particle);
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

