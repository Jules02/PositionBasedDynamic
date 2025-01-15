#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QOpenGLWidget>
#include "context.h"


class DrawArea : public QOpenGLWidget
{
    Q_OBJECT
public:
    DrawArea(int width, int height, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void DrawNewCollider(Collider* collider);

    Context context;
    void renderContext(QPainter *painter, QPaintEvent *event);
    void renderColliders(QPainter *painter);
    void renderObjects(QPainter *painter);

    Vec2 worldToView(Vec2 world_pos);
    Vec2 viewToWorld(Vec2 view_pos);

public slots:
    void animate();

private:
    int width;
    int height;
};

#endif // DRAWAREA_H
