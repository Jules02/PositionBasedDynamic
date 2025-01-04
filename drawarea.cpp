#include "drawarea.h"
#include "Vec2.h"
#include "collider.h"
#include "particle.h"
#include <QPainter>
#include <QMouseEvent>

DrawArea::DrawArea(int _width, int _height, QWidget *parent)
    : width(_width), height(_height), QOpenGLWidget{parent}
{
    this->setFixedSize(this->width, this->height);

    // Add a few colliders, for testing purpose
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{30.0, 250.0}}), normalize(Vec2{{0.0, 1.0}})));
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{500.0, 150.0}}), normalize(Vec2{{-1.0, 1.0}})));
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{50.0, 150.0}}), normalize(Vec2{{2.0, 1.0}})));
    //this->context.addCollider(std::make_unique<SphereCollider>(
    //    worldToView(Vec2{{450.0, 150.0}}), 50.0f));

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
    this->context.updatePhysicalSystem(10);

    QPainter p(this);                       // to be refactored
    this->renderContext(&p, nullptr);

}

void DrawArea::renderContext(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(this->rect(), Qt::black);    // clear canva by painting the entire background

    // TEMPORARY !
    // Render test colliders
    Collider* collider = context.colliders.at(0).get();
    this->renderPlanCollider(painter, dynamic_cast<PlanCollider*>(collider));
    Collider* another_collider = context.colliders.at(1).get();
    this->renderPlanCollider(painter, dynamic_cast<PlanCollider*>(another_collider));
    Collider* new_collider = context.colliders.at(2).get();
    this->renderPlanCollider(painter, dynamic_cast<PlanCollider*>(new_collider));
    //Collider* sphere_collider = context.colliders.at(3).get();
    //this->renderSphereCollider(painter, dynamic_cast<SphereCollider*>(sphere_collider));

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

void DrawArea::renderSphereCollider(QPainter *painter, SphereCollider *collider) {
    Vec2 view_center = worldToView(collider->center);

    QRectF target(view_center[0]-collider->radius,view_center[1]-collider->radius, collider->radius*2, collider->radius*2);
    painter->setPen(QPen(Qt::blue, 3));
    painter->drawEllipse(target);
}

/**
 * @brief Switch from the physical system coordinates to the Qt coordinates.
 */
Vec2 DrawArea::worldToView(Vec2 world_pos) {
    return Vec2{world_pos[0], this->height - world_pos[1]};
}


/**
 * @brief Switch from the Qt coordinates to the physical system coordinates.
 */
Vec2 DrawArea::viewToWorld(Vec2 view_pos) {
    return Vec2{view_pos[0], this->height - view_pos[1]};
}
