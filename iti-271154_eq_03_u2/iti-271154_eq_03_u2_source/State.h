#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>

class State {
public:
    std::string label;
    std::vector<std::pair<std::string, State*>> transitions; // Changed from char to std::string
    std::vector<State*> parents;
    bool is_start;
    bool is_accept;

    State(std::string lbl, bool start = false, bool accept = false);

    void add_transition(std::string symbol, State* state); // Changed from char to std::string

    std::vector<State*> get_parents();
};

#endif /* STATE_H */