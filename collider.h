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
        //if (dot(particle.pos - this->position, this->normal) - particle.radius < 0) {
        if (dot(particle.pos - this->position, this->normal) - 2*particle.radius < 0) {
            Vec2 q_c = particle.pos - dot(particle.pos - this->position, this->normal) * this->normal;
            float C = dot(particle.pos - q_c, this->normal) - 2*particle.radius;
            Vec2 delta = -C * this->normal;
            return StaticConstraint{this->position, this->normal, delta, const_cast<Particle*>(&particle)};
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
    SphereCollider(Vec2 _center, float _radius) : center(_center), radius(_radius) {}

    std::optional<StaticConstraint> checkContact(const Particle& particle) const override {
        // BUG: for the moment, sphere colliders are not implemented properly
        float sdf = length(particle.pos - this->center);
        if (sdf < this->radius + particle.radius) {
            Vec2 normal_c = normalize(particle.pos - this->center);
            return StaticConstraint{particle.pos - sdf*normal_c, normal_c, Vec2{{0.0, 0.0}}, const_cast<Particle*>(&particle)};
        }
        return std::nullopt;
    }

    Vec2 center;
    float radius;
};

#endif // COLLIDER_H
