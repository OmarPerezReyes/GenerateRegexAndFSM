
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QMenu>
#include <QtMath>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QStringList>
#include <QRandomGenerator>
#include <QLineEdit>


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QtWidgets>

class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        resize(1920, 720);

        // Layout horizontal para alinear en la parte superior
        QHBoxLayout* mainLayout = new QHBoxLayout(this);

        // Layout vertical para organizar los botones y el QLineEdit
        QVBoxLayout* buttonLayout = new QVBoxLayout();
        buttonLayout->setAlignment(Qt::AlignTop); // Alinear en la parte superior

        // Botones
        buttonNFA.setText("Generate random NFA");
        buttonDFA.setText("Generate random DFA");
        buttoneNFA.setText("Generate random eNFA");

        // Añadir botones al layout vertical
        buttonLayout->addWidget(&buttonNFA);
        buttonLayout->addWidget(&buttonDFA);
        buttonLayout->addWidget(&buttoneNFA);

        // Crear un QLineEdit
        inputText.setParent(this);

        // Añadir el QLineEdit al layout vertical
        buttonLayout->addWidget(&inputText);

        // Nuevo botón debajo del QLineEdit
        GraphButton.setText("Generate Graph");
        buttonLayout->addWidget(&GraphButton);

        // Añadir el layout vertical al layout horizontal
        mainLayout->addLayout(buttonLayout);

        // Establecer el layout en la ventana principal
        setLayout(mainLayout);

        // Conectar los botones a las funciones correspondientes
        connect(&buttonNFA, &QPushButton::clicked, this, &MainWindow::GenerateRNFA);
        connect(&buttonDFA, &QPushButton::clicked, this, &MainWindow::GenerateRDFA);
        connect(&buttoneNFA, &QPushButton::clicked, this, &MainWindow::GenerateENFA);

        // Conectar el nuevo botón a una función
        connect(&GraphButton, &QPushButton::clicked, this, &MainWindow::GenerateGraph);
    }


    void GenerateRNFA() {
        // Generar un número aleatorio de estados entre 0 y 10
        int numStates = QRandomGenerator::global()->bounded(11); // Rango de 0 a 10

        // Definir los datos del NFA
        activeMethod=0;
        states.clear();
        for (int i = 0; i < numStates; ++i) {
            states.append("s" + QString::number(i));
        }

        QStringList initialStates = {states.first()};
        QStringList acceptingStates = states;
        QStringList alphabet = {"a", "b", "c", "d"}; // Agregar 'c' y 'd'
        transitions.clear(); // Limpiar las transiciones existentes

        // Generar transiciones aleatorias
        for (const QString& state : states) {
            for (const QString& symbol : alphabet) {
                // Obtener un estado aleatorio para la transición
                int randomIndex = QRandomGenerator::global()->bounded(states.size());
                QString nextState = states[randomIndex];

                // Construir la transición y agregarla a la lista de transiciones
                QString transition = state + ":" + symbol + ">" + nextState;
                transitions.append(transition);
            }
        }

        // Construir el texto del NFA aleatorio
        QString randomNFA;
        randomNFA += "#states\n" + states.join("\n") + "\n";
        randomNFA += "#initial\n" + initialStates.join("\n") + "\n";
        randomNFA += "#accepting\n" + acceptingStates.join("\n") + "\n";
        randomNFA += "#alphabet\n" + alphabet.join("\n") + "\n";
        randomNFA += "#transitions\n" + transitions.join("\n");

        // Insertar el texto en el QLineEdit
        inputText.setText(randomNFA);

        // Notificar al usuario que la operación ha sido completada
        QMessageBox::information(this, "Operation Completed", "Random NFA generated successfully!");

        // Forzar el repintado de la ventana para reflejar los cambios
        update();
    }


    void GenerateRDFA() {
        // Generar un número aleatorio de estados entre 0 y 10
        int numStates = QRandomGenerator::global()->bounded(11); // Rango de 0 a 10

        // Definir los datos del RDFA
        activeMethod = 1;
        states.clear();
        for (int i = 0; i < numStates; ++i) {
            states.append("s" + QString::number(i));
        }

        QString initialState = states.first();
        QStringList acceptingStates = states;
        QStringList alphabet = {"a", "b", "c", "d"};
        transitions.clear(); // Limpiar las transiciones existentes

        // Generar transiciones aleatorias
        for (const QString &state : states) {
            for (const QString &symbol : alphabet) {
                // Obtener un estado aleatorio para la transición
                int randomIndex = QRandomGenerator::global()->bounded(states.size());
                QString nextState = states[randomIndex];

                // Construir la transición y agregarla a la lista de transiciones
                QString transition = state + ":" + symbol + ">" + nextState;
                transitions.append(transition);
            }
        }

        // Construir el texto del RDFA aleatorio
        QString randomRDFA;
        randomRDFA += "#states\n" + states.join("\n") + "\n";
        randomRDFA += "#initial\n" + initialState + "\n";
        randomRDFA += "#accepting\n" + acceptingStates.join("\n") + "\n";
        randomRDFA += "#alphabet\n" + alphabet.join("\n") + "\n";
        randomRDFA += "#transitions\n" + transitions.join("\n");

        // Insertar el texto en el QLineEdit
        inputText.setText(randomRDFA);

        // Notificar al usuario que la operación ha sido completada
        QMessageBox::information(this, "Operation Completed", "Random RDFA generated successfully!");

        // Forzar el repintado de la ventana para reflejar los cambios
        update();
    }

    void GenerateENFA() {
        // Definir los datos del eNFA
        activeMethod = 2;

        // Generar un número aleatorio de estados entre 1 y 10
        int numStates = QRandomGenerator::global()->bounded(10) + 1;

        states.clear(); // Limpiar la lista de estados
        QString initialState = "s0"; // Estado inicial predeterminado
        QStringList acceptingStates; // Lista de estados de aceptación
        QStringList alphabet = {"a", "b", "c", "d", "$"}; // Alfabeto con el símbolo de transición epsilon

        // Generar nombres de estados de manera aleatoria
        for (int i = 0; i < numStates; ++i) {
            QString stateName = "s" + QString::number(i);
            states.append(stateName);

            // Seleccionar aleatoriamente algunos estados como estados de aceptación
            if (QRandomGenerator::global()->bounded(4) == 0) {
                acceptingStates.append(stateName);
            }
        }

        transitions.clear(); // Limpiar las transiciones existentes

        // Generar transiciones aleatorias
        for (const QString &state : states) {
            for (const QString &symbol : alphabet) {
                // Obtener un número aleatorio para determinar si la transición es una epsilon-transición
                int random = QRandomGenerator::global()->bounded(10); // Utiliza un rango adecuado según tus necesidades
                if (random < 7) { // Por ejemplo, 70% de las veces se genera una transición regular, y 30% una epsilon-transición
                    // Obtener un estado aleatorio para la transición
                    int randomIndex = QRandomGenerator::global()->bounded(states.size());
                    QString nextState = states[randomIndex];

                    // Construir la transición y agregarla a la lista de transiciones
                    QString transition = state + ":" + symbol + ">" + nextState;
                    transitions.append(transition);
                } else {
                    // Generar una epsilon-transición
                    // Obtener un estado aleatorio para la transición
                    int randomIndex = QRandomGenerator::global()->bounded(states.size());
                    QString nextState = states[randomIndex];

                    // Construir la transición epsilon y agregarla a la lista de transiciones
                    QString transition = state + ":$" + ">" + nextState;
                    transitions.append(transition);
                }
            }
        }

        // Construir el texto del eNFA aleatorio
        QString randomENFA;
        randomENFA += "#states\n" + states.join("\n") + "\n";
        randomENFA += "#initial\n" + initialState + "\n";
        randomENFA += "#accepting\n" + acceptingStates.join("\n") + "\n";
        randomENFA += "#alphabet\n" + alphabet.join("\n") + "\n";
        randomENFA += "#transitions\n" + transitions.join("\n");

        // Insertar el texto en el QLineEdit (o donde corresponda en tu interfaz)
        inputText.setText(randomENFA);

        // Notificar al usuario que la operación ha sido completada
        QMessageBox::information(this, "Operation Completed", "Random eNFA generated successfully!");

        // Forzar el repintado de la ventana para reflejar los cambios
        update();
    }



    void GenerateGraph() {
        // Definir los datos del NFA
        activeMethod = 3;
        QString inputString = inputText.text();

        // Variables para almacenar los datos del grafo
        QStringList states;
        QString initialState;
        QStringList acceptingStates;
        QStringList alphabet;
        QStringList transitionsList;

        // Dividir el texto en líneas
        QStringList lines = inputString.split('\n', Qt::SkipEmptyParts);

        // Iterar sobre cada línea para procesar los datos
        QString currentSection;
        foreach (const QString& line, lines) {
            // Si la línea comienza con "#" es una sección
            if (line.startsWith("#")) {
                currentSection = line;
                continue;
            }

            // Dependiendo de la sección actual, procesar la línea
            if (currentSection == "#states") {
                states.append(line.trimmed());
            } else if (currentSection == "#initial") {
                initialState = line.trimmed();
            } else if (currentSection == "#accepting") {
                acceptingStates.append(line.trimmed());
            } else if (currentSection == "#alphabet") {
                alphabet.append(line.trimmed());
            } else if (currentSection == "#transitions") {
                transitionsList.append(line.trimmed());
            }
        }

        // Ahora que tenemos los datos del grafo, podríamos dibujarlo o hacer otras operaciones con ellos

        // Por ejemplo, podríamos imprimir los datos extraídos para verificar que se hayan procesado correctamente
        qDebug() << "States:" << states;
        qDebug() << "Initial State:" << initialState;
        qDebug() << "Accepting States:" << acceptingStates;
        qDebug() << "Alphabet:" << alphabet;
        qDebug() << "Transitions:" << transitionsList;

        // Forzar el repintado de la ventana para reflejar los cambios
        update();
    }



protected:
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        switch (activeMethod)
        {
        case 0:
            // Dibujar el grafo NFA
            DrawGenerateRNFA(painter);
            break;
        case 1:
            // Dibujar el grafo NFA
            DrawGenerateRDFA(painter);
            break;
        case 2:
            // Dibujar el grafo NFA
            DrawGenerateENFA(painter);
            break;
        case 3:
            DrawGenerateGraphTXT(painter);
            break;
        default:
            break;
        }
    }


    void DrawGenerateRNFA(QPainter &painter) {
        // Definir las posiciones de los estados en el lienzo
        QMap<QString, QPointF> statePositions;
        qreal radius = 30.0; // Radio de los círculos de los estados
        qreal spacing = 150.0; // Espaciado entre los estados
        QMap<QString, int> incomingTransitionsCount;

        // Contar las transiciones entrantes para cada estado
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString toState = parts[1].split(">")[1];
            incomingTransitionsCount[toState]++;
        }

        // Asignar las posiciones de los estados
        for (int i = 0; i < states.size(); ++i) {
            qreal x = (i + 1) * spacing;
            qreal y = height() / 2; // Altura central
            // Si es el estado s1, lo dibujamos más abajo
            if (states[i] == "s1") {
                y += 100.0; // Ajustamos la posición hacia abajo
            }
            statePositions.insert(states[i], QPointF(x, y));
        }

        // Dibujar las transiciones como curvas
        painter.setPen(Qt::blue);
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString fromState = parts[0];
            QString symbol = parts[1].split(">")[0];
            QString toState = parts[1].split(">")[1];

            QPointF fromPos = statePositions.value(fromState);
            QPointF toPos = statePositions.value(toState);

            QPainterPath path;

            if (fromState == toState) {
                // Si el estado de origen y destino son iguales (transición a sí mismo)
                QPointF controlPoint1(fromPos.x() + radius * 1.5, fromPos.y() - radius * 1.5);
                QPointF controlPoint2(fromPos.x() + radius * 1.5, fromPos.y() + radius * 1.5);
                path.moveTo(fromPos + QPointF(radius, 0));
                path.cubicTo(controlPoint1, controlPoint2, fromPos + QPointF(0, radius));
            } else {
                // Calcular los puntos de control para curvar la línea
                qreal controlOffset = 50.0;
                QPointF direction = toPos - fromPos;
                QPointF perpendicular(-direction.y(), direction.x());
                perpendicular /= qSqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());

                // Verificar si la línea debe pasar por encima o por debajo del otro estado
                qreal angle = std::atan2(direction.y(), direction.x()); // Cálculo del ángulo
                qreal offsetY = radius * std::sin(angle);

                QPointF controlPoint1 = fromPos + controlOffset * perpendicular;
                QPointF controlPoint2 = toPos - controlOffset * perpendicular;

                if (offsetY < 0) {
                    // La línea debe pasar por encima del otro estado
                    controlPoint1 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                } else {
                    // La línea debe pasar por debajo del otro estado
                    controlPoint1 += QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 += QPointF(10, radius * incomingTransitionsCount[toState]);
                }

                // Dibujar la curva
                path.moveTo(fromPos);
                path.cubicTo(controlPoint1, controlPoint2, toPos);
            }

            painter.drawPath(path);

            // Dibujar el símbolo de la transición
            QPointF midPoint = path.pointAtPercent(0.3);
            painter.drawText(midPoint, symbol);
        }

        // Dibujar los estados como círculos
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        for (const auto &state : states) {
            QPointF pos = statePositions.value(state);
            painter.drawEllipse(pos, radius, radius);
            painter.drawText(pos, state); // Dibujar el nombre del estado
        }
    }

    void DrawGenerateRDFA(QPainter &painter) {
        // Definir las posiciones de los estados en el lienzo
        QMap<QString, QPointF> statePositions;
        qreal radius = 30.0; // Radio de los círculos de los estados
        qreal spacing = 150.0; // Espaciado entre los estados
        QMap<QString, int> incomingTransitionsCount;

        // Contar las transiciones entrantes para cada estado
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString toState = parts[1].split(">")[1];
            incomingTransitionsCount[toState]++;
        }

        // Asignar las posiciones de los estados
        for (int i = 0; i < states.size(); ++i) {
            qreal x = (i + 1) * spacing;
            qreal y = height() / 2; // Altura central
            // Si es el estado s1, lo dibujamos más abajo
            if (states[i] == "s1") {
                y += 100.0; // Ajustamos la posición hacia abajo
            }
            statePositions.insert(states[i], QPointF(x, y));
        }

        // Dibujar las transiciones como curvas
        painter.setPen(Qt::blue);
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString fromState = parts[0];
            QString symbol = parts[1].split(">")[0];
            QString toState = parts[1].split(">")[1];

            QPointF fromPos = statePositions.value(fromState);
            QPointF toPos = statePositions.value(toState);

            QPainterPath path;

            if (fromState == toState) {
                // Si el estado de origen y destino son iguales (transición a sí mismo)
                QPointF controlPoint1(fromPos.x() + radius * 1.5, fromPos.y() - radius * 1.5);
                QPointF controlPoint2(fromPos.x() + radius * 1.5, fromPos.y() + radius * 1.5);
                path.moveTo(fromPos + QPointF(radius, 0));
                path.cubicTo(controlPoint1, controlPoint2, fromPos + QPointF(0, radius));
            } else {
                // Calcular los puntos de control para curvar la línea
                qreal controlOffset = 50.0;
                QPointF direction = toPos - fromPos;
                QPointF perpendicular(-direction.y(), direction.x());
                perpendicular /= qSqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());

                // Verificar si la línea debe pasar por encima o por debajo del otro estado
                qreal angle = std::atan2(direction.y(), direction.x()); // Cálculo del ángulo
                qreal offsetY = radius * std::sin(angle);

                QPointF controlPoint1 = fromPos + controlOffset * perpendicular;
                QPointF controlPoint2 = toPos - controlOffset * perpendicular;

                if (offsetY < 0) {
                    // La línea debe pasar por encima del otro estado
                    controlPoint1 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                } else {
                    // La línea debe pasar por debajo del otro estado
                    controlPoint1 += QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 += QPointF(10, radius * incomingTransitionsCount[toState]);
                }

                // Dibujar la curva
                path.moveTo(fromPos);
                path.cubicTo(controlPoint1, controlPoint2, toPos);
            }

            painter.drawPath(path);

            // Dibujar el símbolo de la transición
            QPointF midPoint = path.pointAtPercent(0.3);
            painter.drawText(midPoint, symbol);
        }

        // Dibujar los estados como círculos
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        for (const auto &state : states) {
            QPointF pos = statePositions.value(state);
            painter.drawEllipse(pos, radius, radius);
            painter.drawText(pos, state); // Dibujar el nombre del estado
        }
    }


    void DrawGenerateENFA(QPainter &painter) {
        // Definir las posiciones de los estados en el lienzo
        QMap<QString, QPointF> statePositions;
        qreal radius = 30.0; // Radio de los círculos de los estados
        qreal spacing = 150.0; // Espaciado entre los estados
        QMap<QString, int> incomingTransitionsCount;

        // Contar las transiciones entrantes para cada estado
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString toState = parts[1].split(">")[1];
            incomingTransitionsCount[toState]++;
        }

        // Asignar las posiciones de los estados
        for (int i = 0; i < states.size(); ++i) {
            qreal x = (i + 1) * spacing;
            qreal y = height() / 2; // Altura central
            // Si es el estado s1, lo dibujamos más abajo
            if (states[i] == "s1") {
                y += 100.0; // Ajustamos la posición hacia abajo
            }
            statePositions.insert(states[i], QPointF(x, y));
        }

        // Dibujar las transiciones como curvas
        painter.setPen(Qt::blue);
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString fromState = parts[0];
            QString symbol = parts[1].split(">")[0];
            QString toState = parts[1].split(">")[1];

            QPointF fromPos = statePositions.value(fromState);
            QPointF toPos = statePositions.value(toState);

            QPainterPath path;

            if (fromState == toState) {
                // Si el estado de origen y destino son iguales (transición a sí mismo)
                QPointF controlPoint1(fromPos.x() + radius * 1.5, fromPos.y() - radius * 1.5);
                QPointF controlPoint2(fromPos.x() + radius * 1.5, fromPos.y() + radius * 1.5);
                path.moveTo(fromPos + QPointF(radius, 0));
                path.cubicTo(controlPoint1, controlPoint2, fromPos + QPointF(0, radius));
            } else {
                // Calcular los puntos de control para curvar la línea
                qreal controlOffset = 50.0;
                QPointF direction = toPos - fromPos;
                QPointF perpendicular(-direction.y(), direction.x());
                perpendicular /= qSqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());

                // Verificar si la línea debe pasar por encima o por debajo del otro estado
                qreal angle = std::atan2(direction.y(), direction.x()); // Cálculo del ángulo
                qreal offsetY = radius * std::sin(angle);

                QPointF controlPoint1 = fromPos + controlOffset * perpendicular;
                QPointF controlPoint2 = toPos - controlOffset * perpendicular;

                if (offsetY < 0) {
                    // La línea debe pasar por encima del otro estado
                    controlPoint1 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                } else {
                    // La línea debe pasar por debajo del otro estado
                    controlPoint1 += QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 += QPointF(10, radius * incomingTransitionsCount[toState]);
                }

                // Dibujar la curva
                path.moveTo(fromPos);
                path.cubicTo(controlPoint1, controlPoint2, toPos);
            }

            painter.drawPath(path);

            // Dibujar el símbolo de la transición
            QPointF midPoint = path.pointAtPercent(0.3);
            painter.drawText(midPoint, symbol);
        }

        // Dibujar los estados como círculos
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        for (const auto &state : states) {
            QPointF pos = statePositions.value(state);
            painter.drawEllipse(pos, radius, radius);
            painter.drawText(pos, state); // Dibujar el nombre del estado
        }
    }


    void DrawGenerateGraphTXT(QPainter &painter, bool includeDollarSymbol = true) {
        // Definir las posiciones de los estados en el lienzo
        QMap<QString, QPointF> statePositions;
        qreal radius = 30.0; // Radio de los círculos de los estados
        qreal spacing = 150.0; // Espaciado entre los estados
        QMap<QString, int> incomingTransitionsCount;

        // Contar las transiciones entrantes para cada estado
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString toState = parts[1].split(">")[1];
            incomingTransitionsCount[toState]++;
        }

        // Asignar las posiciones de los estados
        for (int i = 0; i < states.size(); ++i) {
            qreal x = (i + 1) * spacing;
            qreal y = height() / 2; // Altura central
            // Si es el estado s1, lo dibujamos más abajo
            if (states[i] == "s1") {
                y += 100.0; // Ajustamos la posición hacia abajo
            }
            statePositions.insert(states[i], QPointF(x, y));
        }

        // Dibujar las transiciones como curvas
        painter.setPen(Qt::blue);
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString fromState = parts[0];
            QString symbols = parts[1].split(">")[0];
            QString toState = parts[1].split(">")[1];

            QPointF fromPos = statePositions.value(fromState);
            QPointF toPos = statePositions.value(toState);

            QPainterPath path;

            // Calcular los puntos de control para curvar la línea
            qreal controlOffset = 50.0;
            QPointF direction = toPos - fromPos;
            QPointF perpendicular(-direction.y(), direction.x());
            perpendicular /= qSqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());

            // Verificar si la línea debe pasar por encima o por debajo del otro estado
            qreal angle = std::atan2(direction.y(), direction.x()); // Cálculo del ángulo
            qreal offsetY = radius * std::sin(angle);

            QPointF controlPoint1 = fromPos + controlOffset * perpendicular;
            QPointF controlPoint2 = toPos - controlOffset * perpendicular;

            // Si la transición va al mismo estado, ajustar los puntos de control para hacer una curva
            if (fromState == toState) {
                qreal offsetX = 50.0;
                qreal offsetY = 50.0;
                controlPoint1 = QPointF(fromPos.x() + offsetX, fromPos.y() - offsetY);
                controlPoint2 = QPointF(fromPos.x() + offsetX, fromPos.y() + offsetY);
            } else {
                if (offsetY < 0) {
                    // La línea debe pasar por encima del otro estado
                    controlPoint1 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                } else {
                    // La línea debe pasar por debajo del otro estado
                    controlPoint1 += QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 += QPointF(10, radius * incomingTransitionsCount[toState]);
                }
            }

            // Dibujar la curva
            path.moveTo(fromPos);
            path.cubicTo(controlPoint1, controlPoint2, toPos);


            painter.drawPath(path);

            // Dibujar el símbolo de la transición (si no es un símbolo de transición epsilon)
            if (includeDollarSymbol || symbols != "$") {
                QStringList symbolList = symbols.split(",");
                QPointF midPoint = path.pointAtPercent(0.5);
                if (symbolList.size() == 2) {
                    qreal yOffset = painter.fontMetrics().height();
                    qreal xOffset1 = painter.fontMetrics().horizontalAdvance(symbolList[0]); // Calcula el ancho de la primera etiqueta
                    qreal xOffset2 = painter.fontMetrics().horizontalAdvance(symbolList[1]); // Calcula el ancho de la segunda etiqueta
                    painter.drawText(midPoint + QPointF(-xOffset1, -yOffset / 2), symbolList[0]); // Dibuja la primera etiqueta hacia la izquierda
                    painter.drawText(midPoint + QPointF(xOffset2 + 2, yOffset / 2), symbolList[1]); // Dibuja la segunda etiqueta hacia la derecha
                } else {
                    // Si hay más de dos símbolos, dibujarlos secuencialmente
                    qreal yOffset = 0;
                    for (const QString &symbol : symbolList) {
                        painter.drawText(midPoint + QPointF(5, yOffset), symbol);
                        yOffset += painter.fontMetrics().height(); // Incrementar el desplazamiento vertical
                    }
                }
            }
        }

        // Dibujar los estados como círculos
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        for (const auto &state : states) {
            QPointF pos = statePositions.value(state);
            painter.drawEllipse(pos, radius, radius);
            painter.drawText(pos, state); // Dibujar el nombre del estado
        }
    }






private:
    QLineEdit inputText;
    QPushButton buttonNFA;
    QPushButton buttonDFA;
    QPushButton buttoneNFA;
    QPushButton GraphButton; // Removed pointer declaration
    QGridLayout layout;

    // Variables para almacenar los estados y transiciones de RandomNFA
    QStringList states;
    QStringList transitions;

    int activeMethod;
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
