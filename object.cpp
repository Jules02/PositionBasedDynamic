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
        painter->setBrush(QBrush(this->color));
        painter->drawEllipse(target);
    }
}
