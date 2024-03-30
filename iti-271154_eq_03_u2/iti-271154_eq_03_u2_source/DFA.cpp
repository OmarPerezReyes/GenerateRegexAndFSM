#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <sstream>
#include "NFA.h"
#include "DFA.h"

DFA::DFA(NFA nfa) : nfa(nfa) {
    constructDFA();
}

std::map<std::string, std::map<std::string, std::string>> DFA::toDict() {
    std::map<std::string, std::map<std::string, std::string>> dict;

    for (const auto& transition : transitions) {
        std::map<std::string, std::string> transitionMap;
        for (const auto& symbolTransition : transition.second) {
            std::string targetStates;
            for (const auto& state : symbolTransition.second) {
                targetStates += state->label + ",";
            }
            if (!targetStates.empty()) {
                targetStates.pop_back(); // Remove the last comma
            }
            transitionMap[symbolTransition.first] = targetStates;
        }
        dict[states[transition.first]] = transitionMap;
    }

    return dict;
}

void DFA::constructDFA() {
    std::set<State*> initialSet = epsilonClosure({nfa.start});
    std::queue<std::set<State*>> workQueue;
    std::map<std::set<State*>, bool> visited;

    states[initialSet] = "S0";
    initialState = "S0";
    workQueue.push(initialSet);
    visited[initialSet] = false;

    while (!workQueue.empty()) {
        std::set<State*> currentState = workQueue.front();
        workQueue.pop();

        if (!visited[currentState]) {
            visited[currentState] = true;
            for (const auto& symbol : nfa.getSymbols()) {
                std::set<State*> nextState = epsilonClosure(move(currentState, symbol));
                if (nextState.empty()) continue;

                if (states.find(nextState) == states.end()) {
                    std::string nextStateLabel = "S" + std::to_string(states.size());
                    states[nextState] = nextStateLabel;
                    workQueue.push(nextState);
                }

                transitions[currentState][symbol] = nextState;
            }
        }
    }

    for (const auto& stateSet : states) {
        for (const auto& state : stateSet.first) {
            if (state->is_accept) {
                acceptingStates.insert(stateSet.second);
                break;
            }
        }
    }

    for (const auto& transition : transitions) {
        for (const auto& symbol : nfa.getSymbols()) {
            alphabet.insert(symbol);
        }
    }
}

std::set<State*> DFA::epsilonClosure(const std::set<State*>& states) {
    std::set<State*> closure;
    std::queue<State*> workQueue;

    for (auto state : states) {
        closure.insert(state);
        workQueue.push(state);
    }

    while (!workQueue.empty()) {
        State* currentState = workQueue.front();
        workQueue.pop();

        for (auto transition : currentState->transitions) {
            if (transition.first == "ε") {
                State* nextState = transition.second;
                if (closure.find(nextState) == closure.end()) {
                    closure.insert(nextState);
                    workQueue.push(nextState);
                }
            }
        }
    }

    return closure;
}

std::set<State*> DFA::move(const std::set<State*>& states, const std::string& symbol) {
    std::set<State*> moveResult;

    for (auto state : states) {
        for (auto transition : state->transitions) {
            if (transition.first == symbol) {
                moveResult.insert(transition.second);
            }
        }
    }

    return moveResult;
}


void DFA::printDFA() {
    std::map<std::string, std::map<std::string, std::string>> dict = toDict();
    std::stringstream output; // Aquí se guardará la salida
  
    output << "-------------------------- DFA  ------------------------------" << std::endl;

    // Imprime el DFA usando el diccionario generado
    output << "#states" << std::endl;
    for (const auto& state : states) {
        output << state.second << std::endl;
    }

    // Imprime el estado inicial
    output << "#initial" << std::endl;
    output << initialState << std::endl;

    // Imprime los estados de aceptación
    output << "#accepting" << std::endl;
    for (const auto& state : acceptingStates) {
        output << state << std::endl;
    }

    // Imprime el alfabeto
    output << "#alphabet" << std::endl;
    for (const auto& symbol : alphabet) {
        output << symbol << std::endl;
    }

    // Imprime las transiciones
    output << "#transitions" << std::endl;
    for (const auto& stateTransition : dict) {
        const std::string& sourceState = stateTransition.first;
        const std::map<std::string, std::string>& transitions = stateTransition.second;
        for (const auto& symbolTarget : transitions) {
            const std::string& symbol = symbolTarget.first;
            const std::string& targetStates = symbolTarget.second;
            std::istringstream iss(targetStates);
            std::string targetState;
            while (std::getline(iss, targetState, ',')) {
                output << sourceState << ":" << symbol << ">" << targetState << std::endl;
            }
        }
    }

    // Convertir el contenido de stringstream a una cadena
    std::string outputString = output.str();

    // Ahora puedes usar 'outputString' como desees, por ejemplo, imprimirlo en la consola
    std::cout << outputString << std::endl;
}
