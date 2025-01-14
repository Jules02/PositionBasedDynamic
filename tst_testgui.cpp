#include "tst_testgui.h"
#include "drawarea.h"
#include "particle.h"

void MyTestGUI::testCase1() {
    // Test the initialization of basic context

    QWidget* fractalDrawing = new QWidget();
    DrawArea *draw_area = new DrawArea(800, 300, fractalDrawing);

    Particle circle(draw_area->viewToWorld({{ 300, 200 }}), {{ 0.0, 0.0 }}, 15, 1.0);
    Object object(Qt::red, circle);
    draw_area->context.objects.push_back(std::move(object));

    QVERIFY(!draw_area->context.objects.empty());
}

void MyTestGUI::testPlanCollider() {
    // Test PlanCollider by creating a physical situation where the associated constraint should be satisfied

    QWidget* fractalDrawing = new QWidget();
    DrawArea *draw_area = new DrawArea(800, 300, fractalDrawing);

    Particle circle(draw_area->viewToWorld({{ 300, 200 }}), {{ 0.0, 0.0 }}, 15, 1.0);
    Object object(Qt::red, circle);
    draw_area->context.objects.push_back(std::move(object));

    auto collider = std::make_unique<PlanCollider>(
        draw_area->worldToView(Vec2{{30.0, 100.0}}), normalize(Vec2{{0.0, 1.0}}));
    auto* colliderPtr = collider.get();
    draw_area->context.addCollider(std::move(collider));
    auto constraint = std::make_unique<StaticConstraint>(*colliderPtr, &circle);

    QVERIFY(constraint->isSatisfied());
}

QTEST_MAIN(MyTestGUI)
#include "tst_testgui.moc"
