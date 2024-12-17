#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QOpenGLWidget>
#include "circle.h"

class DrawArea : public QOpenGLWidget
{
    Q_OBJECT
public:
    DrawArea(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool isGravityOn;

public slots:
    void animate();

private:
    std::vector<Circle> circles;
};

#endif // DRAWAREA_H
