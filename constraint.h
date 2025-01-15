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

#endif // CONSTRAINT_H
