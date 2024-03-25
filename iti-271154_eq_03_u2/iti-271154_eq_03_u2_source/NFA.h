#ifndef NFA_H
#define NFA_H

#include "State.h"
#include <vector>
#include <map>

class NFA {
public:
    State* start;
    State* accept;

    NFA(State* s = nullptr, State* a = nullptr);

    std::vector<State*> getStates();

    std::vector<char> getSymbols();

    std::map<std::string, std::map<char, std::string>> toDict();
};

#endif /* NFA_H */

