#include "context.h"
#include <stdio.h>

Context::Context() {}

const float K_DAMPING = 0.96;

void Context::updatePhysicalSystem(float dt) {

    // Refer to Equations.pdf from the assignement for more details on the different steps
    this->applyExternalForce(dt);             // (5)
    this->dampVelocities();                 // (6)
    this->updateExpectedPosition(dt);         // (7)
    //this->updateNeighbors(dt);
    this->activeConstraints.clear();
    //this->addFluidConstraints(dt);
    this->addDynamicContactConstraints();   // (8)
    this->addStaticContactConstraints();    // (8)
    this->projectConstraints();               // (9-11)
    this->updateVelocityAndPosition(dt);      // (12-15)
    this->applyFriction(dt);                  // (16)

}

void Context::applyExternalForce(float dt) {
    for (Object& object: objects){
        for (auto& particle: object.particles) {
            // GRAVITY
            if (this->isGravityOn) {
                particle->appliedForces = 0.0003*Vec2{0, static_cast<float>(-particle->mass*9.81)};
            }

            particle->velocity = particle->velocity + (dt / particle->mass) * particle->appliedForces;
        }
    }
}

void Context::dampVelocities() {
    for (Object& object: objects){
        for (auto& particle: object.particles) {
            particle->velocity = K_DAMPING*particle->velocity;
        }
    }
}

void Context::updateExpectedPosition(float dt) {
    for (Object& object: objects){
        for (auto& particle: object.particles) {
            particle->expectedPos = particle->pos + dt * particle->velocity;
        }
    }
}

void Context::updateNeighbors() {
    /*
    for (Particle& particle_i : circles) {
        for (Particle& particle_j : circles) {
            if (length(particle_j.pos - particle_i.pos) < 50) {
                particle_i.neighbors.push_back(&particle_j);
            }
        }
    }
    */
}

void Context::addFluidConstraints() {
    /*
    for (Particle& particle_i : circles) {
        auto constraint = std::make_unique<FluidConstraint>(&particle_i);
        if (constraint->isSatisfied()) {
            addConstraint(std::move(constraint));
        }
    }
    */
}

void Context::addDynamicContactConstraints() {
    for (Object& object: objects) {
        for (auto& particle_i : object.particles) {

            for (const auto& particle_j : object.particles) {
                if (&(*particle_j) == particle_i.get()) continue;

                auto link = std::make_unique<LinkConstraint>(*particle_j, particle_i.get());
                if (link->isSatisfied()) {
                    addConstraint(std::move(link));
                }
            }

            for(const Object& otherObject: objects) {
                if (&otherObject == &object) continue;
                for (const auto& particle_j: otherObject.particles) {
                    auto constraint = std::make_unique<DynamicConstraint>(*particle_j, particle_i.get());
                    if (constraint->isSatisfied()) {
                        addConstraint(std::move(constraint));

                        particle_i->isActivated = true;
                    }
                }
            }
        }
    }
}

void Context::addStaticContactConstraints() {

    // GENERATE CONSTRAINTS

    // Check for contact between each collider/particle pair
    for (auto& collider : colliders) {

        for (Object& object: objects){
            for (auto& particle: object.particles) {
                auto constraint = std::make_unique<StaticConstraint>(*collider.get(), particle.get());

                if (constraint->isSatisfied()) {

                    addConstraint(std::move(constraint));

                    particle->isActivated = true;
                }
            }
        }
    }
}

void Context::enforceConstraint(const Constraint& constraint, Particle& particle) {
    particle.expectedPos = particle.expectedPos + constraint.getDelta();
}


void Context::projectConstraints() {
    for (const auto& constraint : activeConstraints) {
        enforceConstraint(*constraint, *constraint->particle);
    }
}


void Context::updateVelocityAndPosition(float dt) {
    for (Object& object: objects){
        for (auto& particle: object.particles) {
            particle->velocity = 1/dt * (particle->expectedPos - particle->pos);
            particle->pos = particle->expectedPos;
        }
    }
}

void Context::applyFriction(float dt) {

}

