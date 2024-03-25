#include "NFA.h"
#include <set>

NFA::NFA(State* s, State* a) : start(s), accept(a) {}

std::vector<State*> NFA::getStates() {
    std::set<State*> visited;
    std::vector<State*> states;
    std::vector<State*> queue;
    queue.push_back(start);
    visited.insert(start);

    while (!queue.empty()) {
        State* state = queue.front();
        queue.erase(queue.begin());
        states.push_back(state);
        for (auto& transition : state->transitions) {
            if (visited.find(transition.second) == visited.end()) {
                visited.insert(transition.second);
                queue.push_back(transition.second);
            }
        }
    }
    return states;
}

std::vector<char> NFA::getSymbols() {
    std::vector<State*> states = getStates();
    std::set<char> symbols;

    for (auto& state : states) {
        for (auto& transition : state->transitions) {
            if (transition.first != 'ϵ') {
                symbols.insert(transition.first);
            }
        }
    }
    std::vector<char> symbolList(symbols.begin(), symbols.end());
    return symbolList;
}

std::map<std::string, std::map<char, std::string>> NFA::toDict() {
    std::vector<State*> states = getStates();
    std::map<std::string, std::map<char, std::string>> result;

    for (auto& state : states) {
        std::map<char, std::string> transitions;
        for (auto& transition : state->transitions) {
            if (transitions.find(transition.first) == transitions.end()) {
                transitions[transition.first] = transition.second->label;
            } else {
                transitions[transition.first] += "," + transition.second->label;
            }
        }
        result[state->label] = transitions;
    }

    return result;
}

