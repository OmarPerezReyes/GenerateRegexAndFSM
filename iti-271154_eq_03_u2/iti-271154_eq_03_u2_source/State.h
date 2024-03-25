#include <string>
#include <vector>
#include <utility> // For std::pair

class State {
public:
    std::string label;
    std::vector<std::pair<std::string, State*>> transitions;
    std::vector<State*> parents;
    bool is_start;
    bool is_accept;

    State(std::string label, bool is_start = false, bool is_accept = true) 
        : label(label), is_start(is_start), is_accept(is_accept) {}

    void add_transition(std::string symbol, State* state) {
        transitions.push_back(std::make_pair(symbol, state));
        this->is_accept = false;
        state->parents.push_back(this);
    }

    std::vector<State*> get_parents() {
        return parents; // Returning a copy is handled automatically in C++
    }

    std::string get_label() const {
        return label;
    }
};

