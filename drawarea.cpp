#include "drawarea.h"
#include "circle.h"
#include <QPainter>
#include <QMouseEvent>
#include <vector>

DrawArea::DrawArea(QWidget *parent)
    : QOpenGLWidget{parent}
{
}

void DrawArea::paintEvent(QPaintEvent *event) {}

void DrawArea::mouseDoubleClickEvent(QMouseEvent *event) {
    int x = event->position().x();
    int y = event->position().y();

    Circle circle(x-30/2, y-30/2, 30, 30);     // -30/2 correction for drawing right under the cursor tip
    this->circles.push_back(circle);
    QPainter p(this);
    QRectF target(circle.x, circle.y, circle.width, circle.height);
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

        for (Circle& circle: this->circles) {
            if (circle.y < 300 - circle.height) {
                circle.y += 1;
            }
            QRectF target(circle.x,circle.y, circle.width, circle.height);
            p.setPen(Qt::green);
            p.setBrush(QBrush(Qt::red));
            p.drawEllipse(target);
        }

        this->update();
    }

}
