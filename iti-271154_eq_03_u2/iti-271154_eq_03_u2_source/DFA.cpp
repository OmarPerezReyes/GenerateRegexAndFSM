#include "DFA.h"
#include <fstream>
#include <bitset>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

// Definición de constantes y variables globales
#define MAX_NFA_STATES 10
#define MAX_ALPHABET_SIZE 10
int D=0; // Número de estados del DFA
int F; // Número de estados finales del NFA
int T; // Número de transiciones en el NFA
using namespace std;

// Representación de un estado NFA
class NFAstate {
public:
    int transitions[MAX_ALPHABET_SIZE][MAX_NFA_STATES];
    NFAstate() {
        for (int i = 0; i < MAX_ALPHABET_SIZE; i++)
            for (int j = 0; j < MAX_NFA_STATES; j++)
                transitions[i][j] = -1;
    }
};

// Representación de un estado DFA
struct DFAstate {
    bool finalState;
    bitset<MAX_NFA_STATES> constituentNFAstates;
    bitset<MAX_NFA_STATES> transitions[MAX_ALPHABET_SIZE];
    int symbolicTransitions[MAX_ALPHABET_SIZE];
};

// Variables globales
set<int> NFA_finalStates;
vector<int> DFA_finalStates;
vector<DFAstate*> DFAstates;
queue<int> incompleteDFAstates;
int N, M; // N -> No. de estados, M -> Tamaño del alfabeto de entrada
NFAstate* NFAstates;

// Función para encontrar el cierre epsilon del estado NFA "state" y almacenarlo en "closure"
void epsilonClosure(int state, bitset<MAX_NFA_STATES>& closure) {
    for (int i = 0; i < N && NFAstates[state].transitions[0][i] != -1; i++) {
        if (closure[NFAstates[state].transitions[0][i]] == 0) {
            closure[NFAstates[state].transitions[0][i]] = 1;
            epsilonClosure(NFAstates[state].transitions[0][i], closure);
        }
    }
}

// Función para encontrar el cierre epsilon de un conjunto de estados NFA "state" y almacenarlo en "closure"
void epsilonClosure(bitset<MAX_NFA_STATES> state, bitset<MAX_NFA_STATES>& closure) {
    for (int i = 0; i < N; i++) {
        if (state[i] == 1) {
            epsilonClosure(i, closure);
        }
    }
}

// Función para devolver un bitset representando el conjunto de estados en los que el NFA podría estar después de moverse
// desde el estado X en el símbolo de entrada A
void NFAmove(int X, int A, bitset<MAX_NFA_STATES>& Y) {
    for (int i = 0; i < N && NFAstates[X].transitions[A][i] != -1; i++) {
        Y[NFAstates[X].transitions[A][i]] = 1;
    }
}

// Función para devolver un bitset representando el conjunto de estados en los que el NFA podría estar después de moverse
// desde el conjunto de estados X en el símbolo de entrada A
void NFAmove(bitset<MAX_NFA_STATES> X, int A, bitset<MAX_NFA_STATES>& Y) {
    for (int i = 0; i < N; i++) {
        if (X[i] == 1) {
            NFAmove(i, A, Y);
        }
    }
}

// Función principal para la conversión de NFA a DFA
void nfa2dfa() {
    D = 1; // Inicializamos el número de estados del DFA
    DFAstates.push_back(new DFAstate); // Creamos el primer estado del DFA
    DFAstates[0]->constituentNFAstates[0] = 1; // Marcamos el primer estado NFA como parte del DFA
    epsilonClosure(0, DFAstates[0]->constituentNFAstates); // Calculamos el cierre epsilon del primer estado NFA

    // Verificamos si el estado resultante es un estado final del NFA
    for (int j = 0; j < N; j++) {
        if (DFAstates[0]->constituentNFAstates[j] == 1 && NFA_finalStates.find(j) != NFA_finalStates.end()) {
            DFAstates[0]->finalState = true; // Marcamos el estado DFA como final si es parte de un estado final NFA
            DFA_finalStates.push_back(0); // Agregamos el estado DFA a la lista de estados finales
            break;
        }
    }

    incompleteDFAstates.push(0); // Agregamos el primer estado DFA a la cola de estados incompletos

    // Procesamos todos los estados incompletos
    while (!incompleteDFAstates.empty()) {
        int X = incompleteDFAstates.front();
        incompleteDFAstates.pop();

        // Procesamos cada símbolo del alfabeto
        for (int i = 1; i <= M; i++) {
            NFAmove(DFAstates[X]->constituentNFAstates, i, DFAstates[X]->transitions[i]); // Calculamos el conjunto de estados NFA alcanzables
            epsilonClosure(DFAstates[X]->transitions[i], DFAstates[X]->transitions[i]); // Calculamos el cierre epsilon del conjunto de estados alcanzables

            // Buscamos un estado DFA equivalente
            int j;
            for (j = 0; j < D; j++) {
                if (DFAstates[X]->transitions[i] == DFAstates[j]->constituentNFAstates) {
                    DFAstates[X]->symbolicTransitions[i] = j; // Marcamos la transición simbólica
                    break;
                }
            }

            // Si no encontramos un estado DFA equivalente, creamos uno nuevo
            if (j == D) {
                DFAstates.push_back(new DFAstate);
                DFAstates[D]->constituentNFAstates = DFAstates[X]->transitions[i];
                DFAstates[D]->finalState = false;
                for (int k = 0; k < N; k++) {
                    if (DFAstates[D]->constituentNFAstates[k] == 1 && NFA_finalStates.find(k) != NFA_finalStates.end()) {
                        DFAstates[D]->finalState = true;
                        DFA_finalStates.push_back(D);
                        break;
                    }
                }
                DFAstates[X]->symbolicTransitions[i] = D;
                incompleteDFAstates.push(D);
                D++;
            }
        }
    }
}


// Función para escribir el DFA resultante en un archivo
void writeDFA() {
    ofstream fout("DFA.txt");
    fout << D << " " << M << "\n" << DFA_finalStates.size();
    for (vector<int>::iterator it = DFA_finalStates.begin(); it != DFA_finalStates.end(); it++)
        fout << " " << *it;
    fout << "\n";
    for (int i = 0; i < D; i++) {
        for (int j = 1; j <= M; j++)
            fout << i << " " << j << " " << DFAstates[i]->symbolicTransitions[j] << "\n";
    }
    fout.close();
}

// Función principal
int main() {
    // Lectura del NFA y construcción del DFA
    ifstream fin("NFA.txt");
    fin >> N >> M;
    NFAstates = new NFAstate[N];
    fin >> F;
    for (int i = 0; i < F; i++) {
        int X;
        fin >> X;
        NFA_finalStates.insert(X);
    }
    fin >> T;
    while (T--) {
        int X, A, Y;
        fin >> X >> A >> Y;
        for (int i = 0; i < Y; i++) {
            int j;
            fin >> j;
            NFAstates[X].transitions[A][i] = j;
        }
    }
    fin.close();

    // Construcción del DFA
    nfa2dfa();

    // Escritura del DFA resultante
    writeDFA();

    return 0;
}
