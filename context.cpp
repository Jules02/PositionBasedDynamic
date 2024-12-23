#include "context.h"

Context::Context() {}

void Context::updatePhysicalSystem(float dt) {

    for (Particle& circle: this->circles) {
        if (circle.pos[1] > 0 + circle.radius*2) {
            this->applyExternalForce(dt, circle);
            circle.velocity = circle.velocity + (dt / circle.mass) * circle.appliedForces;
            circle.expectedPos = circle.pos + dt * circle.velocity;
            this->updateExpectedPosition(dt, circle);
            this->updateVelocityAndPosition(dt, circle);
        }
    }

}

void Context::applyExternalForce(float dt, Particle& particle) {
    if (this->isGravityOn) {
        particle.appliedForces = 0.00001 * Vec2{0, static_cast<float>(-particle.mass*9.81)};
    }
}

void Context::updateExpectedPosition(float dt, Particle& particle) {
    // manage constraints
}

void Context::updateVelocityAndPosition(float dt, Particle& particle) {
    particle.velocity = 1/dt * (particle.expectedPos - particle.pos);
    particle.pos = particle.expectedPos;
}

