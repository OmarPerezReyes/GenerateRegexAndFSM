#include <iostream>
#include <stack>
#include <map>
#include <regex>
#include "POSTFIX.h" // Incluir el archivo de encabezado que contiene la definición de la clase POSTFIX

int main() {
    // Definir una expresión regular
    std::string regex = "(a*b)";

    // Validar la expresión regular
    if (!validateRegex(regex)) {
        std::cerr << "La expresión regular no es válida." << std::endl;
        return 1;
    }

    // Crear una instancia de POSTFIX con la expresión regular
    POSTFIX postfix(regex);

    // Obtener la expresión posfija
    std::string postfix_expr = postfix.get_postfix();

    // Mostrar la expresión posfija
    std::cout << "Expresión regular: " << regex << std::endl;
    std::cout << "Expresión posfija: " << postfix_expr << std::endl;

    return 0;
}

