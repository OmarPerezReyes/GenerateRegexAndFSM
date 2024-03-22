//qt
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDesktopWidget>
//c++
#include <iostream>
#include <string>
#include <stack>
#include <regex>
#include <random>
#include <ctime>
using namespace std;

bool esValida(const string& regex) {
    std::regex patron("^[a-zA-Z0-9$]+(\\+[a-zA-Z0-9$]+)*$");                                    // Expresión regular
    if (!regex_match(regex, patron)) {
        return false;
    }
    
    std::regex operadoresConsecutivos("[+*]{2,}");                                              //verificación de operadores dupliocados
    if (std::regex_search(regex, operadoresConsecutivos)) {
        return false;
    }
    stack<char> pParentesis;
    bool tieneElementos = false; 
    bool ultimoEraParentesis = false; 
    for (char c : regex) {                                                                      // Balanceo de paréntesis y verificación de elementos
        if (c == '(') {
            if (ultimoEraParentesis) {                                                          //si paréntesis al final, no hay operando antes
                return false;
            }
            pParentesis.push(c);
            ultimoEraParentesis = true;                                                         // Marca que el último carácter era un paréntesis abierto
        } else if (c == ')') {
            if (pParentesis.empty() || pParentesis.top() != '(') {                              // Paréntesis cerrado sin abrir
                return false; 
            }
            pParentesis.pop();
            if (ultimoEraParentesis) {                                                          //si paréntesis al final, no hay operando después
                return false;
            }
            ultimoEraParentesis = false; 
        } else if (c != '+' && c != '*' && c != '$') {                                          //válido sin operadores
            tieneElementos = true;
            ultimoEraParentesis = false;
        }
    }
    if (!pParentesis.empty()) {
        return false;
    }
    if (!tieneElementos) {                                                                      // Verifica si hay elementos dentro de los paréntesis
        return false;
    }
    if (!isalnum(regex[0]) && regex[0] != '(') {                                                //comienza con variable?
        return false;
    }
    if (regex.back() == '+') {                                                                  //termina con una decisión?
        return false;
    }
    if (!isalnum(regex.back()) && regex.back() != ')' && regex.back() != '*') {                 //termina con rodilla, valor o variable?
        return false;
    }
    
    return true;
}



string generarCadenaAleatoria(int longitud, const string& caracteres) {                         //cadena aleatoria de longitud y caracteres especificos
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, caracteres.size() - 1);
    string resultado;
    for (int i = 0; i < longitud; ++i) {
        resultado += caracteres[dis(gen)];
    }
    return resultado;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString regex;
    do {
        QDialog dialog;
        dialog.resize(800, 500);
        QDesktopWidget* pantalla = QApplication::desktop();
        int ejex = pantalla->width();
        int ejey = pantalla->height();
        int x = (ejex - dialog.width()) / 2;
        int y = (ejey - dialog.height()) / 2;
        dialog.move(x, y);
        QVBoxLayout layout(&dialog);
        QLineEdit lineEdit;
            lineEdit.setMinimumSize(700, 30);
            lineEdit.setMaximumSize(700, 30);
            lineEdit.setFixedSize(700, 30);
        QPushButton randomButton("Generar Aleatorio");
            randomButton.setMinimumSize(700, 30);
            randomButton.setMaximumSize(700, 30);
            randomButton.setFixedSize(700, 30);
        QPushButton *enfa = new QPushButton("eNFA");
            enfa->setMinimumSize(230, 30);
            enfa->setMaximumSize(230, 30);
            enfa->setFixedSize(230, 30);
            QObject::connect(enfa, &QPushButton::clicked, [&]() {
                 regex = lineEdit.text();
                if (!esValida(regex.toStdString())) {
                    QMessageBox::warning(nullptr, "Error", "Expresion No Valida.\nIntente de Nuevo!");
                //enfa(parametro);
                }
                //enfa(parametro);
            });
        QPushButton *nfa = new QPushButton("NFA");
            nfa->setMinimumSize(230, 30);
            nfa->setMaximumSize(230, 30);
            nfa->setFixedSize(230, 30);
            QObject::connect(nfa, &QPushButton::clicked, [&]() {
                 regex = lineEdit.text();
                if (!esValida(regex.toStdString())) {
                    QMessageBox::warning(nullptr, "Error", "Expresion No Valida.\nIntente de Nuevo!");
                //nfa(parametro);
                }
            });
        QPushButton *dfa = new QPushButton("DFA");
            dfa->setMinimumSize(230, 30);
            dfa->setMaximumSize(230, 30);
            dfa->setFixedSize(230, 30);
            QObject::connect(dfa, &QPushButton::clicked, [&]() {
                 regex = lineEdit.text();
                if (!esValida(regex.toStdString())) {
                    QMessageBox::warning(nullptr, "Error", "Expresion No Valida.\nIntente de Nuevo!");
                //dfa(parametro);
                }
            });
            

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        QHBoxLayout *textLayout = new QHBoxLayout;                                              //cuadro de texto
            textLayout->addWidget(&lineEdit);
        layout.addLayout(textLayout);
        QHBoxLayout *randomLayout = new QHBoxLayout;                                            //boton expresion aleatoria
            randomLayout->addWidget(&randomButton);
        layout.addLayout(randomLayout);
        QHBoxLayout *buttonLayout = new QHBoxLayout;                                            //botones funciones
            buttonLayout->addWidget(enfa);
            buttonLayout->addWidget(nfa);
            buttonLayout->addWidget(dfa);
        layout.addLayout(buttonLayout);

        QHBoxLayout *panelLayout = new QHBoxLayout; 
        QWidget *blankPanel = new QWidget;                                                      // Crear un QWidget para el panel en blanco
            blankPanel->setStyleSheet("background-color: white;"); 
            blankPanel->setMinimumSize(800, 400); 
            blankPanel->setMaximumSize(800, 400); 
        panelLayout->addWidget(blankPanel);
        layout.addLayout(panelLayout);


        layout.addWidget(&buttonBox);
        QObject::connect(&randomButton, &QPushButton::clicked, [&]() {
            string randomized;
            do{
                randomized = generarCadenaAleatoria(6, "abcd+*()$");                            //expresión regular aleatoria
            }while(!esValida(randomized));                                                      //hasta que sea valida
            lineEdit.setText(QString::fromStdString(randomized));
        });
                                                                                             //botones de aceptar y cancelar
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
 
        if (dialog.exec() == QDialog::Accepted) {
            regex = lineEdit.text();
            if (esValida(regex.toStdString())) {
                QMessageBox::information(nullptr, "Validacion", "Expresion Valida.");
                break;
            } else {
                QMessageBox::warning(nullptr, "Error", "Expresion No Valida.\nIntente de Nuevo!");
            }
        } else {
            break;
        }
    } while (true);

    app.exit();
}
