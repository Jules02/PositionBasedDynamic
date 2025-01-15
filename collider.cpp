#include "collider.h"

void PlanCollider::render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) {
    Vec2 view_start = worldToView(this->position - 1000 * perpendicular(this->normal));
    Vec2 view_end = worldToView(this->position + 1000 * perpendicular(this->normal));
    painter->setPen(QPen(Qt::blue, 3));
    painter->drawLine(view_start[0], view_start[1], view_end[0], view_end[1]);
}

void SphereCollider::render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) {
    Vec2 view_center = worldToView(this->center);

    QRectF target(view_center[0]-this->radius,view_center[1]-this->radius, this->radius*2, this->radius*2);
    painter->setPen(QPen(Qt::blue, 3));
    painter->drawEllipse(target);
}
