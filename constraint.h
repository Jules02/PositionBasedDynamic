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

        Vec2 q_c = this->particle->pos - dot(this->particle->pos - collider.position, collider.normal) * collider.normal;
        float C = dot(this->particle->pos - q_c, collider.normal) - this->particle->radius;
        this->delta = -C * collider.normal;

        this->type = INEQUALITY;
    }

    inline float constraintFunction() override {
        return -1 * (dot(this->particle->pos - this->collider.position, this->collider.normal) - this->particle->radius);
    }

    PlanCollider collider;

private:

};

#endif // CONSTRAINT_H
