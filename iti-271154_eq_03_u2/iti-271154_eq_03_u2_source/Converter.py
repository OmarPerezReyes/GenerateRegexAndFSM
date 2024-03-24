class RegexError(Exception):
    pass

class Automaton:
    def __init__(self, states, alphabet, transitions, initial_state, final_states):
        self.states = states
        self.alphabet = alphabet
        self.transitions = transitions
        self.initial_state = initial_state
        self.final_states = final_states

def parse_regex(regex_str):
    # Analiza la expresión regular y genera un autómata finito determinista (DFA)
    states = set()
    alphabet = set()
    transitions = {}
    initial_state = 's0'
    final_states = set()

    state_count = 0

    def get_next_state():
        nonlocal state_count
        state = 's' + str(state_count)
        state_count += 1
        return state

    stack = []
    current_state = initial_state

    for char in regex_str:
        if char == '(':
            stack.append(current_state)
        elif char == ')':
            current_state = stack.pop()
        elif char == '*':
            previous_state = stack[-1]
            new_state = get_next_state()
            states.add(new_state)
            transitions[(previous_state, '')] = new_state
            transitions[(new_state, '')] = previous_state
            stack[-1] = new_state
        elif char == '+':
            pass
        else:
            new_state = get_next_state()
            states.add(new_state)
            transitions[(current_state, char)] = new_state
            current_state = new_state
            alphabet.add(char)

    final_states.add(current_state)

    return Automaton(states, alphabet, transitions, initial_state, final_states)

def build_transition_string(automaton):
    # Construye la cadena de transiciones del autómata
    transition_string = ""
    for state in automaton.states:
        for symbol in automaton.alphabet:
            if (state, symbol) in automaton.transitions:
                next_state = automaton.transitions[(state, symbol)]
                transition_string += f"{state}:{symbol}>{next_state}\n"
    return transition_string

def main():
    regex_str = input("Introduce la expresión regular (en formato de cadena): ")
    try:
        automaton = parse_regex(regex_str)

        transition_str = build_transition_string(automaton)

        print("AUTOMATA:")
        print("#states")
        for state in automaton.states:
            print(state)
        print("#initial")
        print(automaton.initial_state)
        print("#accepting")
        for state in automaton.final_states:
            print(state)
        print("#alphabet")
        for symbol in automaton.alphabet:
            print(symbol)
        print("#transitions")
        print(transition_str)
    except RegexError as e:
        print("Error al procesar la expresión regular:", e)

if __name__ == "__main__":
    main()

