#ifndef COLLIDER_H
#define COLLIDER_H

#include <QtCore/qpoint.h>
#include <QtGui/qpainter.h>
#include "Vec2.h"

class Collider
{
public:
    virtual ~Collider() = default;

    virtual std::array<Vec2, 2> getCollisionParameters(Vec2 particlePos, float particleRadius) = 0;

    //virtual std::optional<StaticConstraint> checkContact(const Particle& particle) const = 0;
};

class PlanCollider: public Collider
{
public:
    PlanCollider(Vec2 _position, Vec2 _normal) : position(_position), normal(_normal) {}

    inline std::array<Vec2, 2> getCollisionParameters(Vec2, float) override {
        return {position, normal};
    }

    /*
    std::optional<StaticConstraint> checkContact(const Particle& particle) const override {
        //if (dot(particle.pos - this->position, this->normal) - particle.radius < 0) {
        if (dot(particle.pos - this->position, this->normal) - 2*particle.radius < 0) {
            Vec2 q_c = particle.pos - dot(particle.pos - this->position, this->normal) * this->normal;
            float C = dot(particle.pos - q_c, this->normal) - 2*particle.radius;
            Vec2 delta = -C * this->normal;
            return StaticConstraint{this->position, this->normal, delta, const_cast<Particle*>(&particle)};
        }
        return std::nullopt;
    }*/

    Vec2 position;
    Vec2 normal;
};

class SphereCollider: public Collider
{
public:
    SphereCollider(Vec2 _center, float _radius) : center(_center), radius(_radius) {}

    inline std::array<Vec2, 2> getCollisionParameters(Vec2 particlePos, float particleRadius) override {
        float sdf = length(particlePos - this->center) - (radius + particleRadius);
        Vec2 n_c = 1/length(particlePos - this->center) * (particlePos - this->center);
        Vec2 p_c = particlePos - sdf * n_c;
        return {p_c, n_c};
    }

    /*std::optional<StaticConstraint> checkContact(const Particle& particle) const override {
        // BUG: for the moment, sphere colliders are not implemented properly
        float sdf = length(particle.pos - this->center);
        if (sdf < this->radius + particle.radius) {
            Vec2 normal_c = normalize(particle.pos - this->center);
            StaticConstraint{particle.pos - sdf*normal_c, normal_c, Vec2{{0.0, 0.0}}, const_cast<Particle*>(&particle)};
        }
        return std::nullopt;
    }*/

    Vec2 center;
    float radius;
};

#endif // COLLIDER_H
