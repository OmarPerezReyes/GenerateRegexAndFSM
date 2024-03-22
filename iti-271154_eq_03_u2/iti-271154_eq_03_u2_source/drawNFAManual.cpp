#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the scene
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 600, 200);

    // Node size and spacing
    const int nodeWidth = 50;
    const int nodeHeight = 50;
    const int nodeSpacing = 150;

    // Create the nodes (states) of the automaton
    QGraphicsEllipseItem *state0 = scene.addEllipse(0, 0, nodeWidth, nodeHeight);
    QGraphicsTextItem *state0Label = new QGraphicsTextItem("0", state0);
    state0Label->setPos((nodeWidth - state0Label->boundingRect().width()) / 2,
                        (nodeHeight - state0Label->boundingRect().height()) / 2);

    QGraphicsEllipseItem *state1 = scene.addEllipse(0, 0, nodeWidth, nodeHeight);
    QGraphicsTextItem *state1Label = new QGraphicsTextItem("1", state1);
    state1Label->setPos((nodeWidth - state1Label->boundingRect().width()) / 2,
                        (nodeHeight - state1Label->boundingRect().height()) / 2);

    QGraphicsEllipseItem *state2 = scene.addEllipse(0, 0, nodeWidth, nodeHeight);
    QGraphicsTextItem *state2Label = new QGraphicsTextItem("2", state2);
    state2Label->setPos((nodeWidth - state2Label->boundingRect().width()) / 2,
                        (nodeHeight - state2Label->boundingRect().height()) / 2);

    QGraphicsEllipseItem *state3 = scene.addEllipse(0, 0, nodeWidth, nodeHeight);
    QGraphicsTextItem *state3Label = new QGraphicsTextItem("3", state3);
    state3Label->setPos((nodeWidth - state3Label->boundingRect().width()) / 2,
                        (nodeHeight - state3Label->boundingRect().height()) / 2);

    // Position the nodes
    state0->setPos(scene.width() / 2 - nodeSpacing - nodeWidth, scene.height() / 2 - nodeHeight / 2);
    state1->setPos(scene.width() / 2 - nodeSpacing / 2 - nodeWidth / 2, scene.height() / 2 - nodeHeight / 2);
    state2->setPos(scene.width() / 2 + nodeSpacing / 2 - nodeWidth / 2, scene.height() / 2 - nodeHeight / 2);
    state3->setPos(scene.width() / 2 + nodeSpacing + nodeWidth / 2, scene.height() / 2 - nodeHeight / 2);

    // Create the transitions between the nodes
    scene.addLine(state0->x() + nodeWidth, state0->y() + nodeHeight / 2, state1->x(), state1->y() + nodeHeight / 2);
    scene.addLine(state1->x() + nodeWidth, state1->y() + nodeHeight / 2, state2->x(), state2->y() + nodeHeight / 2);
    scene.addLine(state2->x() + nodeWidth, state2->y() + nodeHeight / 2, state3->x(), state3->y() + nodeHeight / 2);

    // Create labels for transitions
    QGraphicsTextItem *label_0_1 = scene.addText("a");
    label_0_1->setPos((state0->x() + state1->x()) / 2, (state0->y() + state1->y()) / 2);

    QGraphicsTextItem *label_1_2 = scene.addText("b");
    label_1_2->setPos((state1->x() + state2->x()) / 2, (state1->y() + state2->y()) / 2);

    QGraphicsTextItem *label_2_3 = scene.addText("a");
    label_2_3->setPos((state2->x() + state3->x()) / 2, (state2->y() + state3->y()) / 2);

    // Create the view and show it
    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}

