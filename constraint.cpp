#include "constraint.h"



StaticConstraint::StaticConstraint(const Collider& _collider, Particle* _particle)
    : collider(_collider)
{
    this->particle = _particle;

    // Retrieve the (p_c, n_c) couple, which definition will depend on the type of the  (Plan, Sphere)
    auto param = collider.getCollisionParameters(this->particle->pos, this->particle->radius);
    this->p_c = param[0];
    this->n_c = param[1];

    computeDelta();

    this->type = INEQUALITY;
}

float StaticConstraint::constraintFunction() {
    return -1 * (dot(this->particle->pos - this->p_c, this->n_c) - this->particle->radius);
}

void StaticConstraint::computeDelta() {
    Vec2 q_c = this->particle->pos - dot(this->particle->pos - this->p_c, this->n_c) * this->n_c;
    this->C = dot(this->particle->pos - q_c, this->n_c) - this->particle->radius;
    this->delta = -C*n_c;
}



DynamicConstraint::DynamicConstraint(const Particle& _colliderParticle, Particle* _particle)
    : colliderParticle(_colliderParticle)
{
    this->particle = _particle;

    computeDelta();

    this->type = INEQUALITY;
}

float DynamicConstraint::constraintFunction() { return -C; }

void DynamicConstraint::computeDelta() {
    float distance = length(this->particle->pos - this->colliderParticle.pos);
    this->C = distance - (this->particle->radius + this->colliderParticle.radius);
    float sigma_i = (1/this->colliderParticle.mass) / (1/this->colliderParticle.mass + 1/this->particle->mass) * C;
    this->delta = - sigma_i * 1/distance * (this->particle->pos - this->colliderParticle.pos);
}



LinkConstraint::LinkConstraint(const Particle& _colliderParticle, Particle* _particle)
    : colliderParticle(_colliderParticle)
{
    this->particle = _particle;

    computeDelta();

    this->type = INEQUALITY;
}

float LinkConstraint::constraintFunction() { return C + this->L; }

void LinkConstraint::computeDelta() {
    float distance = length(this->particle->pos - this->colliderParticle.pos);
    this->C = distance - this->L;
    float sigma_i = (1/this->colliderParticle.mass) / (1/this->colliderParticle.mass + 1/this->particle->mass) * this->gamma * C;
    this->delta = - sigma_i * 1/distance * (this->particle->pos - this->colliderParticle.pos);
}



FluidConstraint::FluidConstraint(Particle* _particle)
{
    this->particle = _particle;

    computeDelta();

    this->type = INEQUALITY;
}

float FluidConstraint::constraintFunction() { return -C; }

//KERNELS
float FluidConstraint::W_poly6(Vec2 r, float h) {
    if (length(r) > h) { return 0; }
    return (315 / (64 * M_PI * pow(h, 9))) * pow((pow(h, 2) - pow(length(r),2)),3);
}
float FluidConstraint::W_spiky(Vec2 r, float h) {
    if (length(r) > h) { return 0;}
    return (15 / M_PI * pow(h, 6)) * pow(h - length(r), 3);
}
Vec2 FluidConstraint::grad_W_spiky(Vec2 r, float h) {
    if (length(r) > h) { return Vec2{{0.0, 0.0}}; }
    return (-45 / M_PI * pow(h, 6)) * pow(h - length(r), 2) * normalize(r);
}

const float H_RADIUS = 5;
const float RHO_0 = 100000;
const float EPS = 0.0001;

void FluidConstraint::computeDelta() {

    float C_i = 0.0f;
    float rho_i = 0.0f;
    for (Particle* particle_j : particle->neighbors) {
        if (particle == particle_j) continue;
        rho_i += W_poly6(particle->pos - particle_j->pos, H_RADIUS);
    }
    C_i = rho_i / RHO_0 - 1;

    float denominator_sum = 0.0f;
    for (Particle* particle_k : particle->neighbors) {
        Vec2 grad_p_k_C_i = Vec2{{ 0.0, 0.0 }};
        if (particle_k == particle) {
            Vec2 sum  = Vec2{{ 0.0, 0.0 }};
            for (Particle* particle_l : particle->neighbors) {
                if (particle == particle_l) continue;
                sum = sum + grad_W_spiky(particle->pos - particle_l->pos, H_RADIUS);
                grad_p_k_C_i = 1/RHO_0 * sum;
            }
        } else {
            grad_p_k_C_i = 1/RHO_0 * -grad_W_spiky(particle->pos - particle_k->pos, H_RADIUS);
        }
        denominator_sum += squaredLength(grad_p_k_C_i);
    }
    particle->lambda = - C_i / (denominator_sum + EPS);

    Vec2 delta = Vec2{{ 0.0, 0.0 }};
    for (Particle* particle_j : particle->neighbors) {
        if (particle == particle_j) continue;
        delta = delta + (particle->lambda + particle_j->lambda) * grad_W_spiky(particle->pos - particle_j->pos, H_RADIUS);
    }
    this->delta = 1/RHO_0 * delta;
}
