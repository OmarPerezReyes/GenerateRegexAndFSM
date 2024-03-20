//qt
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
//c++
#include <iostream>
#include <string>
#include <stack>
#include <regex>
using namespace std;

bool esValida(const string& regex) {
    std::regex patron("^[a-zA-Z0-9$+*()]+$");               //expresion regular
    if (!regex_match(regex, patron)) {
        return false;
    }
    stack<char> pParentesis;
    for (char c : regex) {                                  //balanceo de parentesis
        if (c == '(') {
            pParentesis.push(c);
        } else if (c == ')') {
            if (pParentesis.empty() || pParentesis.top() != '(') {        // Paréntesis cerrado sin abrir
                return false; 
            }
            pParentesis.pop();
        }
    }
    if (!pParentesis.empty()) {
        return false;
    }
    if (!isalnum(regex[0]) && regex[0] != '(') {            //comienza con variable?
        return false;
    }
    if (regex.back() == '+') {                              //termina con una decision?
        return false;
    }
    if (!isalnum(regex.back()) && regex.back() != ')' && regex.back() != '*') { //termina con rodilla, valor o variable?
        return false;
    }
    
    return true;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    bool ok;
    QString regex;
    do{
        regex = QInputDialog::getText(nullptr, "Expresion Regular", "Ingresa la Expresion Regular:", QLineEdit::Normal, "", &ok);
        if (!ok || regex.isEmpty()) {                       //si canceló o no ingresó nada
            break; 
        }
        
        if (esValida(regex.toStdString())) {                //si es valida
            QMessageBox::information(nullptr, "Validacion", "Expresion Regular Valida.");
            break;
        } else {
            QMessageBox::warning(nullptr, "Validacion", "Expresion Regular No Valida.\nIntente de Nuevo!");
        }
    } while (true);
    app.exit();
}
