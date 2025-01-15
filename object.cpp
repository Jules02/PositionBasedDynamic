#include "object.h"

void Object::render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) {
    for (const auto& particle: this->particles) {
        Vec2 view_pos = worldToView(particle->pos);
        QRectF target(view_pos[0],view_pos[1], particle->radius*2, particle->radius*2);
        if (particle->isActivated){
            painter->setPen(Qt::green);
        } else {
            painter->setPen(Qt::gray);
        }
        QColor particle_color = this->color;
        // TO BE REFINED
        //particle_color.setAlphaF(particle->mass/this->mass);
        painter->setBrush(QBrush(particle_color));
        painter->drawEllipse(target);
    }
}
