#include <iostream>
#include <stack>
#include <string>

#include <random>
#include <map>
#include <regex>

class POSTFIX {
public:
    std::string regex;
    std::string postfix;

    POSTFIX(std::string regex) : regex(regex) {
        this->postfix = shunt_yard(regex);
    }

    std::string get_postfix() {
        return this->postfix;
    }

private:
    std::string shunt_yard(std::string regex) {
        std::map<char, int> operators = {{'*', 5}, {'+', 4}, {'?', 3}, {'.', 2}, {'|', 1}};
        std::string postfix = "";
        std::stack<char> stack;

        // Replace character classes with alternations
        for (size_t i = 0; i < regex.length(); ++i) {
            char c = regex[i];
            if (c == '[') {
                size_t j = i + 1;
                while (regex[j] != ']') {
                    if (isalnum(regex[j]) && isalnum(regex[j + 1])) {
                        regex.insert(j + 1, "|");
                        ++j;
                    }
                    ++j;
                }
            }
        }

        // Replace hyphens with alternations
        regex = replace_all(regex, '-', '|');

   // Insert concatenation operators
for (size_t i = 0; i < regex.length() - 1; ++i) {
    if ((isalnum(regex[i]) || regex[i] == ')' || regex[i] == '*' || regex[i] == '+' || regex[i] == '?') &&
        (isalnum(regex[i + 1]) || regex[i + 1] == '(' || regex[i + 1] == '[')) {
        regex.insert(i + 1, ".");
        ++i; // Skip the inserted '.' character
    }
}

        // Shunting Yard algorithm
        for (char c : regex) {
            if (c == '(') {
                stack.push(c);
            } else if (c == ')') {
                while (!stack.empty() && stack.top() != '(') {
                    postfix += stack.top();
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();
            } else if (operators.find(c) != operators.end()) {
                while (!stack.empty() && operators[c] <= operators[stack.top()]) {
                    postfix += stack.top();
                    stack.pop();
                }
                stack.push(c);
            } else {
                postfix += c;
            }
        }

        while (!stack.empty()) {
            postfix += stack.top();
            stack.pop();
        }

        return postfix;
    }

    std::string replace_all(std::string str, char from, char to) {
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == from) {
                str[i] = to;
            }
        }
        return str;
    }
};

// Función para validar una expresión regular según las restricciones especificadas
bool validateRegex(const std::string& regex) {
    // Verificar si la expresión regular contiene el símbolo '$'
    if (regex.find('$') != std::string::npos) {
        std::cout << "La expresión regular no puede contener el símbolo '$'." << std::endl;
        return false;
    }

    // Verificar si la expresión regular contiene solo un carácter que no sea número o letra
    if (regex.size() == 1 && !std::isalnum(regex[0])) {
        std::cout << "La expresión regular no puede contener solo un carácter que no sea número o letra." << std::endl;
        return false;
    }

    // Verificar si la expresión regular contiene los caracteres '*' y '+' juntos
    if (regex.find("*+") != std::string::npos || regex.find("+*") != std::string::npos) {
        std::cout << "La expresión regular no puede contener '*' y '+' juntos." << std::endl;
        return false;
    }

    // Verificar si la expresión regular contiene solo '*' o solo '+'
    if (regex == "*" || regex == "+") {
        std::cout << "La expresión regular no puede contener solo '*' o solo '+'." << std::endl;
        return false;
    }

    // Verificar si la expresión regular contiene '*+' o '+*'
    if (regex.find("*+") != std::string::npos || regex.find("+*") != std::string::npos) {
        std::cout << "La expresión regular no puede contener '*+' o '+*'." << std::endl;
        return false;
    }

    try {
        std::regex re(regex);
    } catch (const std::regex_error&) {
        std::cout << "Expresión regular inválida: " << regex << std::endl;
        return false;
    }
    return true;
}

// Función para generar una cadena aleatoria de longitud especificada y caracteres dados
std::string generarCadenaAleatoria(int longitud, const std::string& caracteres) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, caracteres.size() - 1);
    std::string resultado;
    for (int i = 0; i < longitud; ++i) {
        resultado += caracteres[dis(gen)];
    }
    return resultado;
}

std::string generarExpresionRegularAleatoria(int longitud) {

    // Define los caracteres permitidos en la expresión regular
    std::string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Patrones iniciales válidos con letras comodín
    std::vector<std::string> patronesValidos = {
        "d*(a)",       // d seguido de cualquier letra
        "(a+d)*",      // Cualquier letra seguida de d
        "(a|d)*",      // Cualquier letra alternada con d
        "a+(d|b)",     // Una o más letras seguidas de d o b
        "(b|d)*a+"     // d o b seguidos de una o más letras
    };

    // Inicializa la semilla para la generación de números aleatorios
    std::srand(std::time(nullptr));

    // Selecciona un patrón inicial al azar
    std::string regex = patronesValidos[rand() % patronesValidos.size()];

    // Agrega caracteres aleatorios para completar la longitud deseada
    while (regex.length() < static_cast<std::string::size_type>(longitud)) {
        regex += caracteres[rand() % caracteres.size()];
    }

    // Valida la expresión regular generada
    if (!validateRegex(regex)) {
        // Si la expresión regular no es válida, se vuelve a intentar
            srand(time(nullptr));
        return generarExpresionRegularAleatoria(longitud);
    }

    return regex;
}

