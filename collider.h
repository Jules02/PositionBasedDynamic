#ifndef COLLIDER_H
#define COLLIDER_H

#include <QtCore/qpoint.h>
#include <QtGui/qpainter.h>
#include "Vec2.h"

class Collider
{
public:
    virtual ~Collider() = default;

    /**
     * @return the couple (p_c, n_c) defined in Equations.pdf
     */
    virtual std::array<Vec2, 2> getCollisionParameters(Vec2 particlePos, float particleRadius) const = 0;

    virtual void render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) = 0;
};

class PlanCollider: public Collider
{
public:
    PlanCollider(Vec2 _position, Vec2 _normal) : position(_position), normal(_normal) {}
    ~PlanCollider() override = default;

    inline std::array<Vec2, 2> getCollisionParameters(Vec2, float) const override {
        return {position, normal};
    }

    void render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) override;

    Vec2 position;
    Vec2 normal;
};

class SphereCollider: public Collider
{
public:
    SphereCollider(Vec2 _center, float _radius) : center(_center), radius(_radius) {}
    ~SphereCollider() override = default;

    inline std::array<Vec2, 2> getCollisionParameters(Vec2 particlePos, float particleRadius) const override {
        float sdf = length(particlePos - this->center) - (radius + particleRadius);
        Vec2 n_c = 1/length(particlePos - this->center) * (particlePos - this->center);
        Vec2 p_c = particlePos - sdf * n_c;
        return {p_c, n_c};
    }

    void render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) override;

    Vec2 center;
    float radius;
};

#endif // COLLIDER_H
