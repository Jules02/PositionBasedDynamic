#include "drawarea.h"
#include "particle.h"
#include "Vec2.h"
#include <QPainter>
#include <QMouseEvent>
#include <vector>

DrawArea::DrawArea(int _width, int _height, QWidget *parent)
    : width(_width), height(_height), QOpenGLWidget{parent}
{
    this->setFixedSize(this->width, this->height);
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{30.0, 250.0}}), normalize(Vec2{{0.0, -1.0}})));

}

void DrawArea::paintEvent(QPaintEvent *event) {}

void DrawArea::mouseDoubleClickEvent(QMouseEvent *event) {
    float x = event->position().x();
    float y = event->position().y();

    float radius = 15;
    Vec2 view_pos {{ x-radius, y-radius }};      // -radius correction for drawing right under the cursor tip
    Vec2 vel {{ 0.0, 0.0 }};
    float mass = 1.0;
    Particle circle(this->viewToWorld(view_pos), vel, radius, mass);

    this->context.circles.push_back(circle);

    QPainter p(this);                       // to be refactored
    this->renderContext(&p, nullptr);

}

void DrawArea::animate() {
    this->context.addStaticContactConstraints();
    this->context.updatePhysicalSystem(30);

    QPainter p(this);                       // to be refactored
    this->renderContext(&p, nullptr);

}

void DrawArea::renderContext(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(this->rect(), Qt::black);    // clear canva by painting the entire background

    // TEMPORARY !
    Collider* collider = context.colliders.back().get();
    this->renderPlanCollider(painter, dynamic_cast<PlanCollider*>(collider));

    for (Particle circle: this->context.circles) {
        Vec2 view_pos = this->worldToView(circle.pos);
        QRectF target(view_pos[0],view_pos[1], circle.radius*2, circle.radius*2);
        if (circle.isActivated){
            painter->setPen(Qt::green);
        } else {
            painter->setPen(Qt::gray);
        }
        painter->setBrush(QBrush(Qt::red));
        painter->drawEllipse(target);
    }

    this->update();
}

void DrawArea::renderPlanCollider(QPainter *painter, PlanCollider *collider) {
    Vec2 view_start = worldToView(collider->position - 1000 * perpendicular(collider->normal));
    Vec2 view_end = worldToView(collider->position + 1000 * perpendicular(collider->normal));
    painter->setPen(QPen(Qt::blue, 3));
    painter->drawLine(view_start[0], view_start[1], view_end[0], view_end[1]);

    painter->setPen(QPen(Qt::red));
    painter->drawLine(collider->position[0], this->height - collider->position[1],
                      collider->position[0] + collider->normal[0] * 30, this->height - (collider->position[1] + collider->normal[1] * 30));
}

Vec2 DrawArea::worldToView(Vec2 world_pos) {
    return Vec2{world_pos[0], this->height - world_pos[1]};
}


Vec2 DrawArea::viewToWorld(Vec2 view_pos) {
    return Vec2{view_pos[0], this->height - view_pos[1]};
}
