#include <iostream>
#include<math.h>
#include <locale>
#include <vector>
#include <map>
#include <set>
#include "NFA.h"
#include "POSTFIX.h"
#include "DFA.h"
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
int main() {
    
    setlocale(LC_ALL, "en_US.UTF-8");
    try {
   string regex;
    cout << "Ingrese la expresion regular: ";
    cin >> regex;

    if (!validateRegex(regex)) {
        cout << "Expresion regular invalida." << endl;
        return 1;
    }

    POSTFIX postfix(regex);
    cout << "------------------------------------------------------------" << endl;
    cout << "regex: " << regex << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "postfix: " << postfix.get_postfix() << endl;
    cout << "------------------------------------------------------------" << endl;

    NFA nfa = postfix2nfa(postfix.get_postfix());
        std::map<std::string, std::map<std::string, std::string>> dict = nfa.toDict();
        
        nfa2dfa(nfa);                                                                   // Convertir NFA a DFA

        // Imprimir estados
        std::cout << "#states" << std::endl;
        std::set<std::string> states;
        for (auto& pair : dict) {
            states.insert(pair.first);
        }
        for (const auto& state : states) {
            std::cout << state << std::endl;
        }

        // Imprimir estado inicial
        std::cout << "#initial" << std::endl;
        std::cout << nfa.start->label << std::endl;

        // Imprimir estado de aceptación
        std::cout << "#accepting" << std::endl;
        std::cout << nfa.accept->label << std::endl;

        // Imprimir alfabeto
        std::cout << "#alphabet" << std::endl;
        std::vector<std::string> alphabet = nfa.getSymbols();
        for (auto symbol : alphabet) {
            if (!symbol.empty()) { // Ignorar el carácter nulo
                std::cout << symbol << std::endl;
            }
        }

        // Imprimir transiciones
        std::cout << "#transitions" << std::endl;
        for (auto& pair : dict) {
            // Verificar si el estado tiene transiciones
            if (!pair.second.empty()) {
                for (auto& transition : pair.second) {
                    std::cout << pair.first << ":" << transition.first << ">" << transition.second << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Your Regex may be invalid" << std::endl;
    }
    return 0;
}
