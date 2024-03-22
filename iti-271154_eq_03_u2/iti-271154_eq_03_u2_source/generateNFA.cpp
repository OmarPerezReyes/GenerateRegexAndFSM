#include <iostream>
#include <stack>
#include <string>
#include <regex>

using namespace std;

// Clase para representar un NFA básico
class BasicNFA {
public:
    int estadoInicial;
    int estadoFinal;
    char transicion;

    BasicNFA(int inicial, int final, char trans) : estadoInicial(inicial), estadoFinal(final), transicion(trans) {}
};

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

// Función para construir un NFA básico para el carácter vacío '$'
BasicNFA construirNFACaracterVacio() {
    return BasicNFA(0, 0, '$');
}

// Función para construir NFA básicos para cada símbolo alfanumérico en la expresión regular
stack<BasicNFA> construirNFA(const string& regex) {
    stack<BasicNFA> nfas;

    for (char c : regex) {
        if (c == '$') { // Manejar el carácter vacío
            nfas.push(construirNFACaracterVacio());
        } else if (isalnum(c)) { // Manejar caracteres alfanuméricos
            int estadoInicial = 0;
            int estadoFinal = 1;
            nfas.push(BasicNFA(estadoInicial, estadoFinal, c));
        } else if (c == '+') { // Manejar el operador de unión
            if (nfas.size() < 2) {
                cerr << "Error: Operador de unión encontrado pero no hay suficientes elementos en la pila." << endl;
                exit(1); // Salir del programa con un código de error
            }
            BasicNFA nfa2 = nfas.top();
            nfas.pop();
            BasicNFA nfa1 = nfas.top();
            nfas.pop();
            int nuevoEstadoInicial = 0;
            int nuevoEstadoFinal = 1;
            // Transición ε desde el nuevo estado inicial a los estados iniciales de los NFAs para a y B
            nfas.push(BasicNFA(nuevoEstadoInicial, nfa1.estadoInicial, 'E'));
            nfas.push(BasicNFA(nuevoEstadoInicial, nfa2.estadoInicial, 'E'));
            // Transición ε desde los estados finales de los NFAs para a y B al nuevo estado final
            nfas.push(BasicNFA(nfa1.estadoFinal, nuevoEstadoFinal, 'E'));
            nfas.push(BasicNFA(nfa2.estadoFinal, nuevoEstadoFinal, 'E'));
            // Actualizar el estado final de los NFA básicos
            nfas.push(BasicNFA(nfa1.estadoInicial, nfa2.estadoInicial, ' ')); // Transición ε entre los estados iniciales de los NFAs
            nfa1.estadoInicial = nuevoEstadoInicial;
            nfa1.estadoFinal = nuevoEstadoFinal;
            nfas.push(nfa1);
        }
    }

    return nfas;
}

// Función para concatenar dos pilas de NFAs básicos
stack<BasicNFA> concatenarNFAs(stack<BasicNFA>& nfas1, stack<BasicNFA>& nfas2) {
    if (nfas1.empty() || nfas2.empty()) {
        cerr << "Error: No se pueden concatenar pilas de NFAs vacías." << endl;
        exit(1);
    }

    // Tomar el NFA básico final de la primera expresión regular
    BasicNFA nfaFinal1 = nfas1.top();
    nfas1.pop();

    // Tomar el NFA básico inicial de la segunda expresión regular
    BasicNFA nfaInicial2 = nfas2.top();
    nfas2.pop();

    // Conectar el estado final del primer NFA con el estado inicial del segundo NFA usando transición ε
    nfaFinal1.estadoFinal = nfaInicial2.estadoInicial;

    // Unir las pilas de NFAs básicos
    while (!nfas1.empty()) {
        nfas2.push(nfas1.top());
        nfas1.pop();
    }

    // Devolver la pila de NFAs concatenados
    return nfas2;
}

// Función para imprimir los NFA básicos
void imprimirNFA(stack<BasicNFA> nfas) {
    cout << "Construcción de NFA básicos:" << endl;
    int count = 1;
    while (!nfas.empty()) {
        BasicNFA nfa = nfas.top();
        nfas.pop();
        cout << "NFA básico " << count++ << ":" << endl;
        cout << "Estado Inicial: " << nfa.estadoInicial << endl;
        cout << "Estado Final: " << nfa.estadoFinal << endl;
        cout << "Transición: ";
        if (nfa.transicion == ' ') {
            cout << "E" << endl;
        } else {
            cout << nfa.transicion << endl;
        }
    }
}

int main() {
    string regex;
    cout << "Ingrese una expresión regular válida: ";
    cin >> regex;

    if (!esValida(regex)) {
        cout << "La expresión regular ingresada no es válida." << endl;
        return 1;
    }

    stack<BasicNFA> nfaCompleto;
    stack<BasicNFA> nfaTemporal;
    for (char c : regex) {
        if (c == '*') {
            // Realizar la operación de cierre de Kleene
            nfaTemporal = nfaCompleto;
            nfaCompleto = concatenarNFAs(nfaTemporal, nfaTemporal);
        } else if (c == '+') {
            // Realizar la operación de unión
            nfaTemporal = nfaCompleto;
            nfaCompleto = concatenarNFAs(nfaCompleto, nfaTemporal);
        } else if (c == '$') {
            // Manejar el carácter vacío
            nfaTemporal.push(construirNFACaracterVacio());
            nfaCompleto = concatenarNFAs(nfaCompleto, nfaTemporal);
        } else {
            // Construir el NFA básico para el carácter actual y añadirlo a la pila
            nfaCompleto.push(BasicNFA(0, 1, c));
        }
    }

    // Imprimir el NFA final
    imprimirNFA(nfaCompleto);

    return 0;
}

