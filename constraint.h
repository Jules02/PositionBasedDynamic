#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Vec2.h"
#include "collider.h"
#include "particle.h"

class Constraint
{
public:
    
    enum ConstraintType { EQUALITY, INEQUALITY };

    virtual ~Constraint() = default;

    inline bool isSatisfied() {
        switch (type) {
        case EQUALITY:
            return (constraintFunction() == 0);
        case INEQUALITY:
            return (constraintFunction() >= 0);
        }
        
    }

    inline Vec2 getDelta() const { return delta; }

    Particle* particle;

protected:
    Vec2 delta;
    virtual float constraintFunction() = 0;
    ConstraintType type;
    
private:
    //int n;
    float stiffness;

    virtual inline void computeDelta() = 0;

};

class StaticConstraint : public Constraint {
public:
    StaticConstraint(const Collider& _collider, Particle* _particle)
        : collider(_collider)
    {
        this->particle = _particle;

        auto param = collider.getCollisionParameters(this->particle->pos, this->particle->radius);
        this->p_c = param[0];
        this->n_c = param[1];
        computeDelta();

        this->type = INEQUALITY;
    }

    ~StaticConstraint() override = default;

    inline float constraintFunction() override {
        return -1 * (dot(this->particle->pos - this->p_c, this->n_c) - this->particle->radius);
    }


private:
    const Collider& collider;
    Vec2 p_c;
    Vec2 n_c;

    inline void computeDelta() override {
        Vec2 q_c = this->particle->pos - dot(this->particle->pos - this->p_c, this->n_c) * this->n_c;
        float C = dot(this->particle->pos - q_c, this->n_c) - this->particle->radius;
        this->delta = -C*n_c;
    }

};

class DynamicConstraint : public Constraint {
public:
    DynamicConstraint(const Particle& _colliderParticle, Particle* _particle)
        : colliderParticle(_colliderParticle)
    {
        this->particle = _particle;

        computeDelta();

        this->type = INEQUALITY;
    }

    ~DynamicConstraint() override = default;

    inline float constraintFunction() override {
        return -C;
    }

private:
    float C;
    Particle colliderParticle;

    inline void computeDelta() override {
        float distance = length(this->particle->pos - this->colliderParticle.pos);
        this->C = distance - (this->particle->radius + this->colliderParticle.radius);
        float sigma_i = (1/this->colliderParticle.mass) / (1/this->colliderParticle.mass + 1/this->particle->mass) * C;
        this->delta = - sigma_i * 1/distance * (this->particle->pos - this->colliderParticle.pos);
    }
};

class DynamicLink : public Constraint {
public:
    DynamicLink(const Particle& _colliderParticle, Particle* _particle)
        : colliderParticle(_colliderParticle)
    {
        this->particle = _particle;

        computeDelta();

        this->type = INEQUALITY;
    }

    ~DynamicLink() override = default;

    inline float constraintFunction() override {
        return C + this->L;
    }

private:
    float L = 35;
    float gamma = 1.0;

    float C;
    Particle colliderParticle;

    inline void computeDelta() override {
        float distance = length(this->particle->pos - this->colliderParticle.pos);
        this->C = distance - this->L;
        float sigma_i = (1/this->colliderParticle.mass) / (1/this->colliderParticle.mass + 1/this->particle->mass) * this->gamma * C;
        this->delta = - sigma_i * 1/distance * (this->particle->pos - this->colliderParticle.pos);
    }
};

class FluidConstraint : public Constraint {
public:
    FluidConstraint(Particle* _particle)
    {
        this->particle = _particle;

        computeDelta();

        this->type = INEQUALITY;
    }

    ~FluidConstraint() override = default;

    inline float constraintFunction() override {
        return -C;
    }

private:
    float H_RADIUS = 5;
    float RHO_0 = 100000;
    float EPS = 0.0001;

    float C;

    inline void computeDelta() override {

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

    inline float W_poly6(Vec2 r, float h) {
        if (length(r) > h) { return 0; }
        return (315 / (64 * M_PI * pow(h, 9))) * pow((pow(h, 2) - pow(length(r),2)),3);
    }

    inline float W_spiky(Vec2 r, float h) {
        if (length(r) > h) { return 0;}
        return (15 / M_PI * pow(h, 6)) * pow(h - length(r), 3);
    }

    inline Vec2 grad_W_spiky(Vec2 r, float h) {
        if (length(r) > h) { return Vec2{{0.0, 0.0}}; }
        return (-45 / M_PI * pow(h, 6)) * pow(h - length(r), 2) * normalize(r);
    }
};

#endif // CONSTRAINT_H
