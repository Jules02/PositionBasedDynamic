#include "collider.h"

void PlanCollider::render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) {
    Vec2 view_start = worldToView(this->position - 1000 * perpendicular(this->normal));
    Vec2 view_end = worldToView(this->position + 1000 * perpendicular(this->normal));
    //painter->drawLine(view_start[0], view_start[1], view_end[0], view_end[1]);

    // Draw a polygon on the normal's opposite side, to represent the ground
    // TO BE REFACTORED
    Vec2 view_start_2 = worldToView(this->position - 1000 * perpendicular(this->normal) - 500 * this->normal);
    Vec2 view_end_2 = worldToView(this->position + 1000 * perpendicular(this->normal) - 500 * this->normal);
    QPointF points[4] = {
        QPointF(view_start[0], view_start[1]),
        QPointF(view_end[0], view_end[1]),
        QPointF(view_end_2[0], view_end_2[1]),
        QPointF(view_start_2[0], view_start_2[1])
    };
    painter->drawPolygon(points, 4);
}

void SphereCollider::render(QPainter *painter, const std::function<Vec2(const Vec2&)>& worldToView) {
    Vec2 view_center = worldToView(this->center);

    QRectF target(view_center[0]-this->radius,view_center[1]-this->radius, this->radius*2, this->radius*2);
    painter->drawEllipse(target);
}
