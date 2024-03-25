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

    std::vector<std::string> getSymbols(); // Changed from char to std::string

    std::map<std::string, std::map<std::string, std::string>> toDict(); // Changed from char to std::string
};

#endif /* NFA_H */

