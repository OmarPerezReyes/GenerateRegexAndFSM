#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char reg[20];
    char q[20][2];
    int i = 0, j = 1, len, a, b;
    for (a = 0; a < 20; a++)
        for (b = 0; b < 2; b++)
            q[a][b] = 0;

    cout << "Enter a regular expression: ";
    cin >> reg;

    cout << "Given regular expression: " << reg << endl;

    len = strlen(reg);

    while (i < len) {
        if (reg[i] != '|' && reg[i] != '*' && reg[i] != ')') {
            // Crear un estado para el caracter actual
            q[j][0] = j + 1;
            q[j][1] = reg[i]; // Guardar el caracter de transición
            j++;
        } else if (reg[i] == '|') {
            // Operación de unión
            q[0][1] = j + 1; // Transición desde el estado inicial
            q[j][0] = 0; // Transición al estado final
            j++;
        } else if (reg[i] == '*') {
            // Operación de clausura de Kleene
            int temp = q[j][0];
            q[j][0] = j + 1;
            q[j][1] = temp; // Transición desde el estado de clausura al estado inicial
            j++;
        } else if (reg[i] == ')') {
            // Manejo de paréntesis
            q[0][1] = j + 1; // Transición desde el estado inicial
            q[j][0] = 0; // Transición al estado final
            j++;
        }
        i++;
    }

    cout << "Número de estados: " << j << endl;
    cout << "Construcción de NFA básicos:" << endl;

    for (i = 0; i < j; i++) {
        if (q[i][0] != 0) {
            cout << "NFA básico " << i << ":" << endl;
            cout << "Estado Inicial: " << i << endl;
            cout << "Estado Final: " << q[i][0]  << endl;
            cout << "Transición: " << q[i][1] << endl;
        }
    }

    return 0;
}

