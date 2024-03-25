from postfix2nfa import NFA
from nfa2dfa import DFA
from dfa2min import MIN_DFA
from regex2postfix import POSTFIX, validateRegex

def reformat_dfa(dfa):
    # Verificar si los nombres de los estados son largos
    is_long_state = False
    for state in dfa.keys():
        if state != 'startingState' and ' ' in state:
            is_long_state = True
            break

    # Si los nombres de los estados son largos, renombrarlos
    if is_long_state:
        print("RENOMBRADOS:")
        state_mapping = {}
        renamed_dfa = {}    
        threshold = 3  # Umbral de longitud para renombrar estados
        starting_state = dfa['startingState']
        if ' ' in starting_state:
            new_starting_state = starting_state.split()[0]  # Tomar solo la parte antes del primer espacio
            state_mapping[starting_state] = new_starting_state
            renamed_dfa[new_starting_state] = dfa[starting_state]
        else:
            renamed_dfa[starting_state] = dfa[starting_state]

        for state, transitions_data in dfa.items():
            if state != 'startingState' and state != starting_state:
                state_parts = state.split()
                if len(state_parts) > 1:
                    # Tomar solo la parte antes del primer espacio
                    new_state = state_parts[0]
                    state_mapping[state] = new_state
                    renamed_dfa[new_state] = transitions_data
                else:
                    renamed_dfa[state] = transitions_data

        # Actualiza las transiciones para usar los nuevos nombres de estado
        for state, transitions_data in renamed_dfa.items():
            if isinstance(transitions_data, dict):
                updated_transitions = {}
                for symbol, target_state in transitions_data.items():
                    if isinstance(target_state, str) and target_state in state_mapping:
                        updated_transitions[symbol] = state_mapping[target_state]
                    else:
                        updated_transitions[symbol] = target_state
                renamed_dfa[state] = updated_transitions

        renamed_dfa['startingState'] = state_mapping.get(starting_state, starting_state)

        print('REMOBRADA',renamed_dfa)
        return renamed_dfa
    else:
        print("NO RENOMBRADOS")
        return dfa






def convert_to_desired_format(nfa_dict):
    nfa_dict = reformat_dfa(nfa_dict)
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
#Cases: d(d*(b+a)+c), (a*b), (a*b)*, (a+b), b*b+da
    cases = {0: 'b*b+da'}
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
        nfa.visualize(name='output/nfa.gv', view=False)
        print("----------------------------------------------------------------")
        dfa = DFA(nfa)
        print("DFA: ", dfa.toDict())
        dfa.visualize(name='output/dfa.gv', view=False)
        print("----------------------------------------------------------------")
        construct_dfa = convert_to_desired_format(dfa.toDict())
        minDfa = MIN_DFA(dfa)
        print("Minimized DFA: ", minDfa.toDict())
        minDfa.visualize(name='output/min_dfa.gv', view=False)
        print("----------------------------------------------------------------")
        print(f"Structure NFA: \n{convert_to_desired_format(nfa.toDict())}")
        
        try:
            accepting_states = [state for state, data in minDfa.items() if isinstance(data, dict) and data.get('isTerminatingState', False)]
            # Estados de aceptación
            if accepting_states:
            	print(f"Structure DFA: \n{convert_to_desired_format(minDfa.toDict())}")
            else:
                #print(f"Structure DFA:")
                print(f"Structure DFA: \n{construct_dfa}")
        except AttributeError:
            #print(f"Structure DFA:")
            print(f"Structure DFA: \n{construct_dfa}")

    # catch the exception and print it
    except Exception as e:
        print(e)
        print("Your Regex may be invalid")


if __name__ == '__main__':
    main()
