//qt
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
//c++
#include <iostream>
#include <string>
#include <stack>
#include <regex>
#include <random>
#include <ctime>
using namespace std;

bool esValida(const string& regex) {
    std::regex patron("^[a-zA-Z0-9$+*()]+$");                                                   //expresion regular
    if (!regex_match(regex, patron)) {
        return false;
    }
    stack<char> pParentesis;
    for (char c : regex) {                                                                      //balanceo de parentesis
        if (c == '(') {
            pParentesis.push(c);
        } else if (c == ')') {
            if (pParentesis.empty() || pParentesis.top() != '(') {                              //paréntesis cerrado sin abrir
                return false; 
            }
            pParentesis.pop();
        }
    }
    if (!pParentesis.empty()) {
        return false;
    }
    if (!isalnum(regex[0]) && regex[0] != '(') {                                                //comienza con variable?
        return false;
    }
    if (regex.back() == '+') {                                                                  //termina con una decision?
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
        QVBoxLayout layout(&dialog);
        QLineEdit lineEdit;
        QPushButton randomButton("Generar Aleatorio");
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout.addWidget(&lineEdit);                                                            //cuadro de texto
        layout.addWidget(&randomButton);
        layout.addWidget(&buttonBox);
        QObject::connect(&randomButton, &QPushButton::clicked, [&]() {
            string randomized;
            do{
                randomized = generarCadenaAleatoria(5, "abcd+*()$");                            //expresión regular aleatoria
            }while(!esValida(randomized));                                                      //hasta que sea valida
            lineEdit.setText(QString::fromStdString(randomized));
        });

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
