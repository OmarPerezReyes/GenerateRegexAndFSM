#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QStringList>
#include <QRandomGenerator>
#include <QLineEdit>
#include <QGraphicsView>
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
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QStringList>
#include <QRandomGenerator>
#include <QLineEdit>
#include <QGraphicsView>
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
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QtWidgets>
#include <iostream>
#include<math.h>
#include <locale>
#include <vector>
#include <map>
#include <set>
#include "NFA.h"
#include "POSTFIX.h"
using namespace std;

NFA postfix2nfa(std::string postfix) {
    std::vector<NFA> nfaStack;
    int i = 0;

    for (char& c : postfix) {
        if (c == '*') {
            NFA nfa1 = nfaStack.back();
            nfaStack.pop_back();
            State* start = new State("S" + std::to_string(i));
            State* accept = new State("S" + std::to_string(i + 1));
            start->add_transition("ε", nfa1.start);
            start->add_transition("ε", accept);
            nfa1.accept->add_transition("ε", start);
            nfa1.accept->add_transition("ε", accept);
            nfaStack.push_back(NFA(start, accept));
            i += 2;
        } else if (c == '.') {
            NFA nfa2 = nfaStack.back();
            nfaStack.pop_back();
            NFA nfa1 = nfaStack.back();
            nfaStack.pop_back();
            nfa1.accept->add_transition("ε", nfa2.start);
            nfaStack.push_back(NFA(nfa1.start, nfa2.accept));
        } else if (c == '|') {
            NFA nfa2 = nfaStack.back();
            nfaStack.pop_back();
            NFA nfa1 = nfaStack.back();
            nfaStack.pop_back();
            State* start = new State("S" + std::to_string(i));
            State* accept = new State("S" + std::to_string(i + 1));
            start->add_transition("ε", nfa1.start);
            start->add_transition("ε", nfa2.start);
            nfa1.accept->add_transition("ε", accept);
            nfa2.accept->add_transition("ε", accept);
            nfaStack.push_back(NFA(start, accept));
            i += 2;
        } else if (c == '+') {
            NFA nfa1 = nfaStack.back();
            nfaStack.pop_back();
            State* start = new State("S" + std::to_string(i));
            State* accept = new State("S" + std::to_string(i + 1));
            start->add_transition("ε", nfa1.start);
            nfa1.accept->add_transition("ε", start);
            nfa1.accept->add_transition("ε", accept);
            nfaStack.push_back(NFA(start, accept));
            i += 2;
        } else if (c == '?') {
    NFA nfa1 = nfaStack.back();
    nfaStack.pop_back();
    State* start = new State("S" + std::to_string(i));
    State* accept = new State("S" + std::to_string(i + 1));
    start->add_transition("ε", nfa1.start);
    start->add_transition("ε", accept);
    nfa1.accept->add_transition("ε", accept);
    nfaStack.push_back(NFA(start, accept));
    i += 2;
} else {
            State* start = new State("S" + std::to_string(i));
            State* accept = new State("S" + std::to_string(i + 1));
            start->add_transition(std::string(1, c), accept);
            nfaStack.push_back(NFA(start, accept));
            i += 2;
        }
    }

    return nfaStack.back();
}
class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumWidth(1900); // Establecer el ancho mínimo de la ventana

        // Configuración del diseño horizontal para alinear en la parte superior
        QHBoxLayout* mainLayout = new QHBoxLayout(this);

        // Configuración del diseño vertical para organizar los botones y el QLineEdit
        QVBoxLayout* buttonLayout = new QVBoxLayout();
        buttonLayout->setAlignment(Qt::AlignTop); // Alinear en la parte superior

        // Configuración de los botones
        buttonNFA.setText("Generate NFA");


        // Agregar los botones al diseño vertical
        buttonLayout->addWidget(&buttonNFA);


        // Crear un QLineEdit
        inputText.setParent(this);

        // Agregar el QLineEdit al diseño vertical
        buttonLayout->addWidget(&inputText);


        // Agregar el diseño vertical al diseño horizontal
        mainLayout->addLayout(buttonLayout);

        // Establecer el diseño en la ventana principal
        setLayout(mainLayout);

        // Conectar los botones a las funciones correspondientes
        connect(&buttonNFA, &QPushButton::clicked, this, &MainWindow::GenerateRNFA);


        // Inicializar el color para los estados finales
        finalStateColor = Qt::magenta; // Color magenta para los estados finales
    }

    void GenerateRNFA() {
        QString input = inputText.text(); // Obtener el texto del QLineEdit
        QString qString = inputText.text(); // Obtener el texto del QLineEdit

 try {
   string regex=input.toStdString();

    if (!validateRegex(regex)) {
        cout << "Expresion regular invalida." << endl;
        //return 1;
    }

    POSTFIX postfix(regex);
    cout << "------------------------------------------------------------" << endl;
    cout << "regex: " << regex << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "postfix: " << postfix.get_postfix() << endl;
    cout << "------------------------------------------------------------" << endl;

    NFA nfa = postfix2nfa(postfix.get_postfix());
        std::map<std::string, std::map<std::string, std::string>> dict = nfa.toDict();
        
        //nfa2dfa(nfa);                                                                   // Convertir NFA a DFA
    std::stringstream output; // Aquí se guardará la salida
// Imprimir estados
    output << "#states" << std::endl;
    std::set<std::string> states;
    for (auto& pair : dict) {
        states.insert(pair.first);
    }
    for (const auto& state : states) {
        output << state << std::endl;
    }

    // Imprimir estado inicial
    output << "#initial" << std::endl;
    output << nfa.start->label << std::endl;

    // Imprimir estado de aceptación
    output << "#accepting" << std::endl;
    output << nfa.accept->label << std::endl;

    // Imprimir alfabeto
    output << "#alphabet" << std::endl;
    std::vector<std::string> alphabet = nfa.getSymbols();
    for (auto symbol : alphabet) {
        if (!symbol.empty()) { // Ignorar el carácter nulo
            output << symbol << std::endl;
        }
    }

   // Imprimir transiciones
    output << "#transitions" << std::endl;
    for (auto& pair : dict) {
        // Verificar si el estado tiene transiciones
        if (!pair.second.empty()) {
            for (auto& transition : pair.second) {
                std::istringstream iss(transition.second);
                std::string targetState;
                while (std::getline(iss, targetState, ',')) {
                    output << pair.first << ":" << transition.first << ">" << targetState << std::endl;
                }
            }
        }
    }

    // Convertir el contenido de stringstream a una cadena

std::string outputString = output.str();

// Encontrar la posición del último endl
size_t lastEndlPos = outputString.find_last_of("\n");

// Truncar la cadena hasta la posición del último endl
if (lastEndlPos != std::string::npos) {
    outputString = outputString.substr(0, lastEndlPos);
}

    // Ahora puedes usar 'outputString' como desees, por ejemplo, imprimirlo en la consola
    //std::cout << outputString << std::endl;
    qString = QString::fromStdString(outputString);


    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Your Regex may be invalid" << std::endl;
    }
	    std::cout << qString.toStdString() << std::endl;
        // Limpiar los datos existentes
        activeMethod = 0;
        states.clear();
        transitions.clear();
        finalStates.clear(); // Limpiar los estados finales

        // Procesar el texto para construir el NFA
        QStringList lines = qString.split("\n");
        
        QString section;
        for (const QString &line : lines) {
            if (line.startsWith("#")) {
                section = line;
                continue;
            }
            if (section == "#states") {
                states.append(line);
            } else if (section == "#initial") {
                QStringList initialStates = line.split(",");
                for (const QString &state : initialStates) {
                    states.append(state);
                }
            } else if (section == "#accepting") {
                QStringList acceptingStates = line.split(",");
                for (const QString &state : acceptingStates) {
                    states.append(state);
                    finalStates.insert(state); // Guardar los estados finales
                }
            } else if (section == "#alphabet") {
            } else if (section == "#transitions") {
                transitions.append(line);
            }
        }
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

        default:
            break;
        }
    }
    void DrawGenerateRNFA(QPainter &painter) {
        //Definir las posiciones de los estados en el lienzo
        QMap<QString, QPointF> statePositions;
        qreal radius = 30.0; //Radio de los círculos de los estados
        qreal spacing = 150.0; //Espaciado entre los estados
        QMap<QString, int> incomingTransitionsCount;

        //Contar las transiciones entrantes para cada estado
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString toState = parts[1].split(">")[1];
            incomingTransitionsCount[toState]++;
        }

        //Asignar las posiciones de los estados
        for (int i = 0; i < states.size(); ++i) {
            qreal x = (i + 1) * spacing;
            qreal y = height() / 2; //Altura central
            //Si es el estado s1, lo dibujamos más abajo
            if (states[i] == "s1") {
                y += 100.0; //Ajustamos la posición hacia abajo
            }
            statePositions.insert(states[i], QPointF(x, y));
        }

        //Dibujar las transiciones como curvas
        painter.setPen(Qt::black);
        for (const auto &transition : transitions) {
            QStringList parts = transition.split(":");
            QString fromState = parts[0];
            QString toState = parts[1].split(">")[1];
            QStringList symbols = parts[1].split(">")[0].split(","); //Obtener todas las letras del alfabeto

            QPointF fromPos = statePositions.value(fromState);
            QPointF toPos = statePositions.value(toState);

            QPainterPath path;
            QPointF controlPoint1, controlPoint2;

            if (fromState == toState) {
                //Si el estado de origen y destino son iguales (transición a sí mismo)
                qreal controlOffset = 65.0;
                QPointF controlPoint1(fromPos.x() + controlOffset, fromPos.y() - controlOffset);
                QPointF controlPoint2(fromPos.x() + controlOffset, fromPos.y() + controlOffset);
                path.moveTo(fromPos + QPointF(radius, 0));
                path.cubicTo(controlPoint1, controlPoint2, fromPos + QPointF(0, radius));
            }
            else {
                //Calcular los puntos de control para curvar la línea
                qreal controlOffset = 50.0;
                QPointF direction = toPos - fromPos;
                QPointF perpendicular(-direction.y(), direction.x());
                perpendicular /= qSqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());

                //Verificar si la línea debe pasar por encima o por debajo del otro estado
                qreal angle = std::atan2(direction.y(), direction.x()); //Cálculo del ángulo
                qreal offsetY = radius * std::sin(angle);

                controlPoint1 = fromPos + controlOffset * perpendicular;
                controlPoint2 = toPos - controlOffset * perpendicular;

                if (offsetY < 0) {
                    //La línea debe pasar por encima del otro estado
                    controlPoint1 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 -= QPointF(10, radius * incomingTransitionsCount[toState]);
                } else {
                    //La línea debe pasar por debajo del otro estado
                    controlPoint1 += QPointF(10, radius * incomingTransitionsCount[toState]);
                    controlPoint2 += QPointF(10, radius * incomingTransitionsCount[toState]);
                }

                //Dibujar la curva
                path.moveTo(fromPos);
                path.cubicTo(controlPoint1, controlPoint2, toPos);

                //Calcular un punto a lo largo de la línea de la transición, un poco antes del final
                qreal t = 0.75; //Porcentaje del camino a lo largo de la transición
                QPointF arrowPos = path.pointAtPercent(t);

                //Calcular la dirección de la transición
                QPointF transitionDirection = toPos - fromPos;
                transitionDirection /= qSqrt(transitionDirection.x() * transitionDirection.x() + transitionDirection.y() * transitionDirection.y());

                //Calcular la posición del triángulo un poco antes del final
                arrowPos += -10 * transitionDirection; //Mover el triángulo 10 unidades hacia atrás a lo largo de la dirección de la transición

                //Calcular las posiciones de los vértices del triángulo
                QPointF arrowP1 = arrowPos + QPointF(0, 0);  //Punto en la parte superior
                QPointF arrowP2 = arrowPos + QPointF(-10, -5); //Punto en la esquina inferior izquierda
                QPointF arrowP3 = arrowPos + QPointF(-10, 5);  //Punto en la esquina superior izquierda

                //Dibujar el triángulo negro
                QPainterPath arrow;
                arrow.moveTo(arrowP1);
                arrow.lineTo(arrowP2);
                arrow.lineTo(arrowP3);
                arrow.lineTo(arrowP1);
                painter.fillPath(arrow, Qt::black);
            }

            painter.drawPath(path);

            //Agrupar todos los símbolos de transición
            QString concatenatedSymbols = symbols.join(",");

            //Dibujar el símbolo de la transición
            if (!concatenatedSymbols.isEmpty()) {
                qreal totalSymbols = symbols.size();
                qreal step = 1.0 / totalSymbols; //Paso para cada símbolo
                qreal t = 0.5; //Punto inicial a lo largo de la curva

                for (const auto &symbol : symbols) {
                    //Obtener el punto a lo largo de la curva para dibujar el símbolo
                    QPointF symbolPos = path.pointAtPercent(t);

                    //Dibujar el símbolo en la posición calculada
                    painter.drawText(symbolPos, symbol);

                    t += step; //Incrementar el punto a lo largo de la curva para el próximo símbolo
                }
            }
        }


        //Definir el color de relleno para los estados finales
        painter.setBrush(finalStateColor);

        //Dibujar los estados como círculos
        painter.setPen(Qt::black);
        for (const auto &state : states) {
            QPointF pos = statePositions.value(state);

            //Si el estado es final, dibujar un círculo más grande en magenta y luego uno más pequeño en blanco
            if (finalStates.contains(state)) {
                //Dibujar el círculo grande en magenta
                painter.setBrush(finalStateColor);
                painter.drawEllipse(pos, radius + 5, radius + 5);

                //Dibujar el círculo más pequeño en blanco
                painter.setBrush(Qt::white);
                painter.drawEllipse(pos, radius, radius);
            } else {
                //Si no es un estado final, dibujar solo un círculo normal en blanco
                painter.setBrush(Qt::white);
                painter.drawEllipse(pos, radius, radius);
            }

            painter.drawText(pos, state); //Dibujar el nombre del estado
        }
    }


private:
    QLineEdit inputText;
    QPushButton buttonNFA;
    QPushButton buttoneNFA;
    QPushButton GraphButton;
    QGridLayout layout;

    QStringList states;
    QStringList transitions;
    QSet<QString> finalStates; //Guardar los estados finales

    int activeMethod;

    //Variables para el dibujo
    QMap<QString, QPointF> statePositions;
    qreal radius = 30.0; //Radio de los círculos de los estados
    QColor finalStateColor; //Color de relleno para los estados finales
};




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
