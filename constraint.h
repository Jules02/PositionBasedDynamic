#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Vec2.h"
#include "collider.h"
#include "particle.h"

class Constraint
{
public:
    
    enum ConstraintType { EQUALITY, INEQUALITY };
    
    //Constraint();

    virtual ~Constraint() = default;

    inline bool isSatisfied() {
        switch (type) {
        case EQUALITY:
            return (constraintFunction() == 0);
        case INEQUALITY:
            return (constraintFunction() >= 0);
        }
        
    }

    Particle* particle;
    Vec2 delta;
    virtual float constraintFunction() = 0;
    ConstraintType type;
    
private:

    //int n;
    float stiffness;

};

class StaticConstraint : public Constraint {
public:
    StaticConstraint(const PlanCollider& _collider, Particle* _particle)
        : collider(_collider)
    {
        this->particle = _particle;

        /*Vec2 q_c = this->particle->pos - dot(this->particle->pos - collider.position, collider.normal) * collider.normal;
        float C = dot(this->particle->pos - q_c, collider.normal) - this->particle->radius;
        this->delta = -C * collider.normal;*/
        auto param = collider.getCollisionParameters(this->particle->pos, this->particle->radius);
        this->p_c = param[0];
        this->n_c = param[1];
        Vec2 q_c = this->particle->pos - dot(this->particle->pos - this->p_c, this->n_c) * this->n_c;
        float C = dot(this->particle->pos - q_c, n_c) - this->particle->radius;
        this->delta = -C*n_c;

        this->type = INEQUALITY;
    }

    ~StaticConstraint() override = default;

    inline float constraintFunction() override {
        return -1 * (dot(this->particle->pos - this->p_c, this->n_c) - this->particle->radius);
    }

    PlanCollider collider;
    Vec2 p_c;
    Vec2 n_c;

private:

};

class DynamicConstraint : public Constraint {
public:
    DynamicConstraint(const Particle& _colliderParticle, Particle* _particle)
        : colliderParticle(_colliderParticle)
    {
        this->particle = _particle;

        float distance = length(this->particle->pos - colliderParticle.pos);
        this->C = distance - (this->particle->radius + colliderParticle.radius);
        float sigma_i = (1/colliderParticle.mass) / (1/colliderParticle.mass + 1/this->particle->mass) * C;
        this->delta = - sigma_i * 1/distance * (this->particle->pos - colliderParticle.pos);

        this->type = INEQUALITY;
    }

    ~DynamicConstraint() override = default;

    inline float constraintFunction() override {
        return -C;
    }

    float C;
    Particle colliderParticle;
};

#endif // CONSTRAINT_H
