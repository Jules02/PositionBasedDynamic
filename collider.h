#ifndef COLLIDER_H
#define COLLIDER_H

#include <QtCore/qpoint.h>
#include <QtGui/qpainter.h>
#include <optional>
#include "particle.h"
#include "staticconstraint.h"

class Collider
{
public:
    virtual ~Collider() = default;

    virtual std::optional<StaticConstraint> checkContact(const Particle& particle) const = 0;
};

class PlanCollider: public Collider
{
public:
    PlanCollider(Vec2 _position, Vec2 _normal) : position(_position), normal(_normal) {}

    std::optional<StaticConstraint> checkContact(const Particle& particle) const override {
        if (dot(particle.pos - this->position, this->normal) + 2*particle.radius > 0) {
            return StaticConstraint{this->position, this->normal, const_cast<Particle*>(&particle)};
        }
        return std::nullopt;
    }

    Vec2 position;
    Vec2 normal;

private:
    //Vec2 position;
    //Vec2 normal;
};

class SphereCollider: public Collider
{
public:

};

#endif // COLLIDER_H
