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
    float stiffness;
    virtual inline void computeDelta() = 0;

};



/**
 * @brief Represents the constraint generated by the collision between a particle and a static element of the scene, eg. the ground.
 */
class StaticConstraint : public Constraint {
public:
    StaticConstraint(const Collider& _collider, Particle* _particle);

    ~StaticConstraint() override = default;

    float constraintFunction() override;


private:
    const Collider& collider;
    float C;
    Vec2 p_c;
    Vec2 n_c;

    void computeDelta() override;

};



/**
 * @brief Represents the constraint generated by the collision between two particles (repulsive)
 */
class DynamicConstraint : public Constraint {
public:
    DynamicConstraint(const Particle& _colliderParticle, Particle* _particle);

    ~DynamicConstraint() override = default;

    float constraintFunction() override;

private:
    float C;
    Particle colliderParticle;

    void computeDelta() override;
};



/**
 * @brief Represents the constraint generated by the interaction between two particles (attractive)
 */
class LinkConstraint : public Constraint {
public:
    LinkConstraint(const Particle& _colliderParticle, Particle* _particle);

    ~LinkConstraint() override = default;

    float constraintFunction() override;

private:
    float L = 30;
    float gamma = 0.6;

    float C;
    Particle colliderParticle;

    void computeDelta() override;
};



/**
 * @brief In the context of a fluid, represents the constraint generated by the interaction between a particle and its neighbors
 */
class FluidConstraint : public Constraint {
public:
    FluidConstraint(Particle* _particle);

    ~FluidConstraint() override = default;

    float constraintFunction() override;

private:
    float C;

    //KERNELS
    float W_poly6(Vec2 r, float h);
    float W_spiky(Vec2 r, float h);
    Vec2 grad_W_spiky(Vec2 r, float h);

    void computeDelta() override;

};

#endif // CONSTRAINT_H
