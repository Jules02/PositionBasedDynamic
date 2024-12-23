#include "drawarea.h"
#include "particle.h"
#include "Vec2.h"
#include <QPainter>
#include <QMouseEvent>
#include <vector>

DrawArea::DrawArea(QWidget *parent)
    : QOpenGLWidget{parent}
{
}

void DrawArea::paintEvent(QPaintEvent *event) {}

void DrawArea::mouseDoubleClickEvent(QMouseEvent *event) {
    float x = event->position().x();
    float y = event->position().y();

    float radius = 15;
    Vec2 pos {{ x-radius, y-radius }};   // -radius correction for drawing right under the cursor tip
    Vec2 vel {{ 0.0, 0.0 }};

    Particle circle(pos, vel, radius, 0.0);
    this->circles.push_back(circle);
    QPainter p(this);
    QRectF target(circle.pos[0], circle.pos[1], circle.radius*2, circle.radius*2);
    p.setPen(Qt::green);
    p.setBrush(QBrush(Qt::red));
    p.drawEllipse(target);
    this->update();

    this->animate();
}

void DrawArea::animate() {

    if (isGravityOn) {
        QPainter p(this);
        p.fillRect(this->rect(), Qt::black);    // clear canva by painting the entire background

        for (Particle& circle: this->circles) {
            if (circle.pos[1] < 300 - circle.radius*2) {
                // TODO: use circle.pos[1]+= 1 instead
                Vec2 dy {{0, 1}};
                circle.pos = circle.pos + dy;
            }
            QRectF target(circle.pos[0],circle.pos[1], circle.radius*2, circle.radius*2);
            p.setPen(Qt::green);
            p.setBrush(QBrush(Qt::red));
            p.drawEllipse(target);
        }

        this->update();
    }

}
