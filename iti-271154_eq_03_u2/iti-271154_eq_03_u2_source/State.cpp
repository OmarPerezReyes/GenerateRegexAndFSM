#include "State.h"

State::State(std::string lbl, bool start, bool accept) : 
    label(lbl), is_start(start), is_accept(accept) {}

void State::add_transition(std::string symbol, State* state) { // Changed from char to std::string
    transitions.push_back(std::make_pair(symbol, state));
    is_accept = false;
    state->parents.push_back(this);
}

std::vector<State*> State::get_parents() {
    return parents;
}
