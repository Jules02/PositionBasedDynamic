#include "drawarea.h"
#include "Vec2.h"
#include "collider.h"
#include "particle.h"
#include "object.h"
#include <QPainter>
#include <QMouseEvent>

QColor generateRandomColor() {
    int red = std::rand() % 256;
    int green = std::rand() % 256;
    int blue = std::rand() % 256;

    return QColor(red, green, blue);
}

DrawArea::DrawArea(int _width, int _height, QWidget *parent)
    : width(_width), height(_height), QOpenGLWidget{parent}
{
    std::srand(std::time(nullptr));

    this->setFixedSize(this->width, this->height);

    // Add two first objects
    float radius = 15;
    float mass = 1.0;
    Particle circle1(this->viewToWorld({{ 360, 40 }}), {{ 0.0, 0.0 }}, radius, mass);
    Particle circle2(this->viewToWorld({{ 360, 70 }}), {{ 0.0, 0.0 }}, radius, mass);
    Particle circle3(this->viewToWorld({{ 390, 40 }}), {{ 0.0, 0.0 }}, radius, mass);
    Particle circle4(this->viewToWorld({{ 390, 70 }}), {{ 0.0, 0.0 }}, radius, mass);
    Object object(generateRandomColor(), circle1, circle2, circle3, circle4);
    this->context.objects.push_back(std::move(object));
    Particle circle5(this->viewToWorld({{ 165, 65 }}), {{ 0.0, 0.0 }}, radius, mass);
    Particle circle6(this->viewToWorld({{ 160, 35 }}), {{ 0.0, 0.0 }}, radius, mass);
    Particle circle7(this->viewToWorld({{ 190, 35 }}), {{ 0.0, 0.0 }}, radius, mass);
    Object object2(generateRandomColor(), circle5, circle6, circle7);
    this->context.objects.push_back(std::move(object2));

    // Add a few colliders, for testing purpose
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{30.0, 400.0}}), normalize(Vec2{{0.0, 1.0}})));
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{600.0, 150.0}}), normalize(Vec2{{-1.0, 1.0}})));
    this->context.addCollider(std::make_unique<PlanCollider>(
        worldToView(Vec2{{50.0, 150.0}}), normalize(Vec2{{2.5, 1.0}})));
    this->context.addCollider(std::make_unique<SphereCollider>(
        worldToView(Vec2{{440.0, 170.0}}), 50.0f));
    this->context.addCollider(std::make_unique<SphereCollider>(
        worldToView(Vec2{{25.0, 200.0}}), 100.0f));
}

void DrawArea::paintEvent(QPaintEvent *event) {}

void DrawArea::mouseDoubleClickEvent(QMouseEvent *event) {
    float x = event->position().x();
    float y = event->position().y();

    float radius = 15;
    float mass = 1.0;
    Vec2 view_pos {{ x-radius, y-radius }};      // -radius correction for drawing right under the cursor tip
    Vec2 vel {{ 0.0, 0.0 }};
    Particle circle(this->viewToWorld(view_pos), vel, radius, mass);

    // As it stands, a double click will create a new object, made of a single particle
    Object object(generateRandomColor(), circle);
    this->context.objects.push_back(std::move(object));

    QPainter p(this);                       // to be refactored
    this->renderContext(&p, nullptr);

}

void DrawArea::animate() {
    this->context.updatePhysicalSystem(10);

    QPainter p(this);                       // to be refactored
    this->renderContext(&p, nullptr);

}

void DrawArea::renderContext(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(this->rect(), QColor(215, 214, 213));    // clear canva by painting the entire background
    painter->setPen(QPen(QColor(134, 132, 130)));
    painter->setBrush(QBrush(QColor(134, 132, 130)));

    renderColliders(painter);
    renderObjects(painter);

    this->update();
}

void DrawArea::renderColliders(QPainter *painter) {
    for (const auto& collider: context.colliders) {
        collider->render(painter, [this](const Vec2& pos) { return worldToView(pos); }); // Passing a function pointer to worldToView is probably not the best solution. These utilities could be defined somewhere else.
    }
}

void DrawArea::renderObjects(QPainter *painter) {
    for (Object& object: context.objects){
        object.render(painter, [this](const Vec2& pos) { return worldToView(pos); });   // Same comment as with renderColliders
    }
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


void DrawNewCollider(Collider* collider){
}


void DrawArea::mousePressEvent(QMouseEvent* event){
        if (!(this->context.click.has_value())) { //if the optional vec2 doesnt have a value it means that it is the first click
            this->context.click=Vec2{static_cast<float>(event->pos().x()), static_cast<float>(event->pos().y())};
        }
        else {
            Vec2 first_click= this->context.click.value();
            Vec2 second_click=Vec2{static_cast<float>(event->pos().x()), static_cast<float>(event->pos().y())};
            first_click= worldToView(first_click); // converting the positions to the view
            second_click= worldToView(second_click);
            this->context.click= second_click - first_click; //directing vector in the optional vector

            if (this->context.addPlanCollidersOn){  //the button is on
                Vec2 normal= perpendicular(this->context.click.value()); // transforming it in a normal vector to use the addcolliders function
                std::unique_ptr<PlanCollider> new_collider= std::make_unique<PlanCollider> (first_click, normal);
                this->context.addCollider(std::move(new_collider));
            }

            else if (this->context.addSphereColliderOn){
                float norm= squaredLength(this->context.click.value()); // getting the radius which is the norm of the directing vector
                std::unique_ptr<SphereCollider> new_collider= std::make_unique<SphereCollider> (first_click,sqrt(norm));
                this->context.addCollider(std::move(new_collider));
            }
            this->context.click.reset(); // reseting the optional vector when already two clicks
        }
}
