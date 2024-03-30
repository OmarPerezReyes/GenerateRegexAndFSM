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
    setMinimumWidth(1250); // Establecer el ancho mínimo de la ventana
    setMinimumHeight(600); // Establecer el ancho mínimo de la ventana

    // Configuración del diseño principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Configuración del diseño horizontal para organizar los elementos de entrada y etiquetas
    QHBoxLayout* inputRowLayout = new QHBoxLayout();

    // Configuración del diseño horizontal para organizar los botones
    QHBoxLayout* buttonRowLayout = new QHBoxLayout();

    // Configuración del diseño horizontal para mostrar cadena de construcción
    QHBoxLayout* outputStringRowLayout = new QHBoxLayout();

    // Configuración de las etiquetas
    QLabel* labelRegex = new QLabel("Regex:");
    QLabel* labelPosfix = new QLabel("Posfix:");

    // Configuración de QLineEdit
    inputRegex.setParent(this);
    outputPostfix.setReadOnly(true); // Hacer que el QLineEdit sea no editable
    outputConstructionFSM.setParent(this);

    // Configuración de los botones
    buttonRandomRegex.setText("Generate Random Regex");
    buttonGenerateNFA.setText("Generate NFA");
    buttonGenerateDFA.setText("Generate DFA");
    buttonDrawFSM.setText("Draw FSM");

    // Agregar elementos de entrada y etiquetas al diseño horizontal
    inputRowLayout->addWidget(labelRegex);
    inputRowLayout->addWidget(&inputRegex);
    inputRowLayout->addWidget(labelPosfix);
    inputRowLayout->addWidget(&outputPostfix);
    inputRowLayout->addWidget(&buttonRandomRegex);

    // Agregar botones al diseño horizontal
    buttonRowLayout->addWidget(&buttonGenerateNFA);
    buttonRowLayout->addWidget(&buttonGenerateDFA);
    buttonRowLayout->addWidget(&buttonDrawFSM);

    // Agregar zona impresión
    outputStringRowLayout->addWidget(&outputConstructionFSM);

    // Añadir los diseños horizontales al diseño principal
    mainLayout->addLayout(inputRowLayout);
    mainLayout->addLayout(buttonRowLayout);
    mainLayout->addLayout(outputStringRowLayout);

    // Añadir espaciador vertical para empujar todo hacia arriba
    mainLayout->addStretch(1);

    // Establecer el diseño en la ventana principal
    setLayout(mainLayout);

        // Conectar los botones a las funciones correspondientes
        connect(&buttonGenerateNFA, &QPushButton::clicked, this, &MainWindow::GenerateRNFA);
        connect(&buttonRandomRegex, &QPushButton::clicked, this, &MainWindow::GenerateREGEX);
        connect(&buttonDrawFSM, &QPushButton::clicked, this, &MainWindow::DibujarNFA);
        // Inicializar el color para los estados finales
        finalStateColor = Qt::magenta; // Color magenta para los estados finales
    }
    void DibujarNFA(){
    QString text = outputConstructionFSM.text();
    limpiarDatos(text);
            update();
    }
    // Función para limpiar los datos existentes
void limpiarDatos(const QString qString) {
   
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
}

void GenerateREGEX(){
 // Obtener una semilla única basada en el tiempo actual
    srand(time(nullptr));
	QString randomRegex = QString::fromStdString(generarExpresionRegularAleatoria(5));
inputRegex.setText(randomRegex);
}
    void GenerateRNFA() {
        QString input = inputRegex.text(); // Obtener el texto del QLineEdit
        QString qString = inputRegex.text(); // Obtener el texto del QLineEdit

 try {
 string regex = input.toStdString();

if (!validateRegex(regex)) {
    // Mostrar mensaje de error con QMessageBox
    QMessageBox::warning(this, "Expresión Regular Inválida", "La expresión regular ingresada es inválida.");

    // Borrar el texto de inputRegex
    inputRegex.clear();

    // Salir de la función o realizar otras acciones según sea necesario
    return;
}

    POSTFIX postfix(regex);
    cout << "------------------------------------------------------------" << endl;
    cout << "regex: " << regex << endl;
    cout << "------------------------------------------------------------" << endl;
    
    cout << "postfix: " << postfix.get_postfix() << endl;
    cout << "------------------------------------------------------------" << endl;
QString postfixString = QString::fromStdString(postfix.get_postfix());
outputPostfix.setText(postfixString);

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
bool hasEpsilon = false; // Variable para verificar si ya se ha incluido epsilon en el alfabeto
for (auto symbol : alphabet) {
    if (!symbol.empty()) { // Ignorar el carácter nulo
        output << symbol << std::endl;
        if (symbol == "ε") {
            hasEpsilon = true;
        }
    }
}

// Si "ε" no está en el alfabeto, añadirlo
if (!hasEpsilon) {
    output << "ε" << std::endl;
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


    //std::cout << outputString << std::endl;
    qString = QString::fromStdString(outputString);
outputConstructionFSM.setText(qString);

        limpiarDatos(qString);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Your Regex may be invalid" << std::endl;
    }
	    std::cout << qString.toStdString() << std::endl;
     
        update();
    }


protected:
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        qreal x = -100; // coordenada x de inicio
    qreal y = 100; // coordenada y de inicio
        switch (activeMethod)
        {
        case 0:
            // Dibujar el grafo NFA
        DrawGenerateRNFA(painter, x, y);

            break;

        default:
            break;
        }
    }
    void DrawGenerateRNFA(QPainter &painter, qreal startX, qreal startY) {
    
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

         // Asigna las posiciones de los estados
    for (int i = 0; i < states.size(); ++i) {
        qreal x = startX + (i + 1) * spacing;
        qreal y = startY + height() / 2; // Ajusta según la posición de inicio y la altura del lienzo
        // Si es el estado s1, lo dibujamos más abajo
        if (states[i] == "s1") {
            y += 100.0; // Ajustamos la posición hacia abajo
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
    QLineEdit outputConstructionFSM;
    QLineEdit inputRegex;
    QLineEdit outputPostfix;
    QPushButton buttonRandomRegex;
    QPushButton buttonGenerateNFA;
    QPushButton buttonGenerateDFA;
    QPushButton buttonDrawFSM;
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
