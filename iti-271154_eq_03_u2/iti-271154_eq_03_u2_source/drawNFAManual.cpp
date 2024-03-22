#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QStringList>
#include <QDebug>

// Clase para representar un NFA básico
class BasicNFA {
public:
    int estadoInicial;
    int estadoFinal;
    QString transicion;

    BasicNFA(int inicial, int final, QString trans) : estadoInicial(inicial), estadoFinal(final), transicion(trans) {}
};

// Función para generar el NFA a partir de la entrada
void generarNFA(QGraphicsScene& scene, const QStringList& entrada) {
    // Limpiar la escena antes de agregar nuevos elementos
    scene.clear();

    // Node size and spacing
    const int nodeWidth = 50;
    const int nodeHeight = 50;
    const int nodeSpacing = 150;

    // Variable para almacenar la cantidad de estados
    int numEstados = 0;

    // Buscar la línea que contiene el número de estados
    QString numEstadosStr;
    for (const QString& line : entrada) {
        if (line.startsWith("Número de estados:")) {
            numEstadosStr = line.split(":")[1].trimmed();
            numEstados = numEstadosStr.toInt();
            break;
        }
    }

    if (numEstados == 0) {
        qDebug() << "Error: No se pudo encontrar el número de estados en la entrada.";
        return;
    }

    // Crear los nodos (estados) del autómata
    QList<QGraphicsEllipseItem*> states;
    QList<QGraphicsTextItem*> stateLabels;

    // Agregar estados según el número especificado
    for (int i = 0; i < numEstados; ++i) {
        QGraphicsEllipseItem *state = scene.addEllipse(0, 0, nodeWidth, nodeHeight);
        QGraphicsTextItem *stateLabel = new QGraphicsTextItem(QString::number(i), state);
        stateLabel->setPos((nodeWidth - stateLabel->boundingRect().width()) / 2,
                            (nodeHeight - stateLabel->boundingRect().height()) / 2);
        state->setPos((i + 1) * nodeSpacing, scene.height() / 2 - nodeHeight / 2);
        states.append(state);
        stateLabels.append(stateLabel);
    }

//Dibujar transiciones
for (int i = 0; i < entrada.size(); ++i) {
    if (entrada[i].startsWith("NFA básico")) {
        QString estadoInicialStr = entrada[i + 1].split(":")[1].trimmed();
        QString estadoFinalStr = entrada[i + 2].split(":")[1].trimmed();
        QString transicion = entrada[i + 3].split(":")[1].trimmed();
        qDebug() << "Estado Inicial:" << estadoInicialStr << "Estado Final:" << estadoFinalStr << "Transición:" << transicion;
        int estadoInicial = estadoInicialStr.toInt();
        int estadoFinal = estadoFinalStr.toInt();

        // Verificar que los estados estén dentro del rango
        if (estadoInicial < 0 || estadoInicial >= numEstados || estadoFinal < 0 || estadoFinal >= numEstados) {
            qDebug() << "Error: Estado fuera de rango.";
            continue;
        }

        // Crear la transición entre los estados
        QGraphicsLineItem *transition = scene.addLine(states[estadoInicial]->x() + nodeWidth, states[estadoInicial]->y() + nodeHeight / 2,
                                                       states[estadoFinal]->x(), states[estadoFinal]->y() + nodeHeight / 2);
        QGraphicsTextItem *label = scene.addText(transicion);
        label->setPos((states[estadoInicial]->x() + states[estadoFinal]->x()) / 2, (states[estadoInicial]->y() + states[estadoFinal]->y()) / 2);
    }
}

}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the scene
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 800, 200);

    // Entrada de ejemplo
    QStringList entrada;
    entrada << "Número de estados: 4" << "Construcción de NFA básicos:" << "NFA básico 1:" << "Estado Inicial: 0" << "Estado Final: 1" << "Transición: a"
            << "NFA básico 2:" << "Estado Inicial: 1" << "Estado Final: 2" << "Transición: b" << "NFA básico 3:" << "Estado Inicial: 2"
            << "Estado Final: 3" << "Transición: a";

    // Generar el NFA a partir de la entrada
    generarNFA(scene, entrada);

    // Create the view and show it
    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}
