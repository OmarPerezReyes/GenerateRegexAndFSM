#ifndef DFA_H
#define DFA_H

#include <string>
#include <unordered_map>
#include <vector>
#include <deque>

class State {
public:
    std::string label;
    std::unordered_map<std::string, State*> transitions;
    bool isAcceptingState;

    State(const std::string& label, bool isAcceptingState = false);
    ~State();
};

class DFA {
private:
    std::unordered_map<std::string, State*> states;
    State* startState;

public:
    DFA(State* startState);
    ~DFA();

    void addState(State* state);
    State* getStateByLabel(const std::string& label);
    std::vector<State*> getStatesByLabel(const std::vector<std::string>& labels);
    std::vector<std::string> getSymbols();
    std::string epsilonClosure(const std::vector<State*>& states);
    std::vector<State*> move(const std::vector<State*>& states, const std::string& symbol);
    void nfa2dfa(/* Parámetros necesarios para la conversión */);
    std::unordered_map<std::string, State*> toDict();
    void visualize(const std::string& name = "output/dfa.gv", bool view = false);
};

#endif // DFA_H
