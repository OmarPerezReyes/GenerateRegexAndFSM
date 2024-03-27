#ifndef DFA_H
#define DFA_H

#include "NFA.h"
#include <iostream>
#include <queue>
#include <set>
#include <map>

class DFA {
public:
    DFA(NFA nfa);
    std::map<std::string, std::map<std::string, std::string>> toDict();
    void printDFA();

private:
    NFA nfa;
    std::map<std::set<State*>, std::string> states;
    std::string initialState;
    std::set<std::string> acceptingStates;
    std::set<std::string> alphabet;
    std::map<std::set<State*>, std::map<std::string, std::set<State*>>> transitions;

    void constructDFA();
    std::set<State*> epsilonClosure(const std::set<State*>& states);
    std::set<State*> move(const std::set<State*>& states, const std::string& symbol);
};

#endif /* DFA_H */
