from postfix2nfa import NFA
#from nfa2dfa import DFA
#from dfa2min import MIN_DFA
from regex2postfix import POSTFIX, validateRegex

def convert_to_desired_format(nfa_dict):
    states = [state for state in nfa_dict.keys() if state != 'startingState']  # Obtenemos la lista de estados excluyendo el estado inicial
    initial_state = nfa_dict['startingState']  # Estado inicial
    accepting_states = [state for state, data in nfa_dict.items() if isinstance(data, dict) and data.get('isTerminatingState', False)]  # Estados de aceptación
    alphabet = set()
    transitions = []

    # Obtenemos el alfabeto y las transiciones
    for state, transitions_data in nfa_dict.items():
        if isinstance(transitions_data, dict):
            for symbol, target_state in transitions_data.items():
                if symbol != 'isTerminatingState':
                    alphabet.add(symbol)
                    transitions.append(f"{state}:{symbol}>{target_state}")

    # Convertimos todo a strings y los ordenamos
    states_str = '\n'.join(states)
    accepting_states_str = '\n'.join(accepting_states)
    alphabet_str = '\n'.join(sorted(list(alphabet)))
    transitions_str = '\n'.join(sorted(transitions))

    return f"#states\n{states_str}\n#initial\n{initial_state}\n#accepting\n{accepting_states_str}\n#alphabet\n{alphabet_str}\n#transitions\n{transitions_str}"

def main():
#d(d*(b+a)+c), (a*b), (a*b)*
    cases = {0: '(a*b)*'}
    regex = cases[0]

    # regex = input("Enter regular expression: ")
    if not validateRegex(regex):
        return
    try:
        postfix = POSTFIX(regex)
        print("----------------------------------------------------------------")
        print("regex:", regex)
        print("----------------------------------------------------------------")
        print("postfix: ", postfix.get_postfix())
        print("----------------------------------------------------------------")
        nfa = NFA(postfix=postfix.get_postfix())
        #print("NFA: ", nfa.toDict())
        nfa.visualize(name='output/nfa.gv', view=True)
        '''print("----------------------------------------------------------------")
        dfa = DFA(nfa)
        print("DFA: ", dfa.toDict())
        dfa.visualize(name='output/dfa.gv', view=False)
        print("----------------------------------------------------------------")
        minDfa = MIN_DFA(dfa)
        print("Minimized DFA: ", minDfa.toDict())
        minDfa.visualize(name='output/min_dfa.gv', view=False)
        print("----------------------------------------------------------------")'''
        print(convert_to_desired_format(nfa.toDict()))

    # catch the excpetion anr print it
    except Exception as e:
        print(e)
        print("Your Regex may be invalid")


if __name__ == '__main__':
    main()
