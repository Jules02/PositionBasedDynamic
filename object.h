#ifndef OBJECT_H
#define OBJECT_H

#include "particle.h"
#include <QColor>

class Object
{
public:
    Object() = default;

    template <typename... Particles>
    Object(QColor color, Particles&&... particles)
        : color(color)
    {
        (addParticle(std::make_unique<Particle>(particles)), ...);
    }

    QColor color;
    std::vector<std::unique_ptr<Particle>> particles;

    inline void addParticle(std::unique_ptr<Particle> particle) { particles.push_back(std::move(particle)); }
};

#endif // OBJECT_H
