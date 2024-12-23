#include "context.h"

Context::Context() {}

void Context::updatePhysicalSystem(float dt) {
    if (this->isGravityOn) {
        for (Particle& circle: this->circles) {
            if (circle.pos[1] < 300 - circle.radius*2) {
                // TODO: use circle.pos[1]+= 1 instead
                Vec2 dy {{0, 1}};
                circle.pos = circle.pos + dy;
            }
        }
    }
}

