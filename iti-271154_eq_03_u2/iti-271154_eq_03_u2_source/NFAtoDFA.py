from collections import deque
import sys

class DFA:

    def __init__(self, nfa):
        self.nfa = nfa
        self.states = self.nfa2dfa(nfa)

    
    # Funciones de utilidad
    def getStateByLabel(self, label):
        return self.nfa.getStateByLabel(label)
    
    def getStatesByLabel(self, labels):
        return self.nfa.getStatesByLabel(labels)
    
    def getSymbols(self):
        return self.nfa.getSymbols() - {'ε'}  # Excluimos ε del alfabeto
    
    # Funciones DFA
    def epsilonClosure(self, states):
        closure = set(states)
        stack = list(states)
        while stack:
            state = stack.pop()
            for symbol, next_state in state.transitions:
                if next_state not in closure:
                    if symbol == "ε":
                        closure.add(next_state)
                        stack.append(next_state)
        closureList =  list(closure)
        closureList.sort(key=lambda x: x.label)
        closureString = ''
        for state in closureList:
            closureString += ' ' + state.label
        return closureString[1:]
    
    def move(self, nfa, states, symbol):
        move_states = set()
        statesList = states.split()
        states = []
        for label in statesList:
            states.append(nfa.getStateByLabel(label))
        for state in states:
            for s, next_state in state.transitions:
                if s == symbol:
                    move_states.add(next_state)
        return move_states
    
    # Conversión de NFA a DFA
    def nfa2dfa(self, nfa):
        nfaStates = nfa.getStates()
        symbols = nfa.getSymbols()
        start_state = self.epsilonClosure([nfaStates[0]])
        dfa_states = {'startingState': start_state}
        queue = deque([start_state])
        seen = set([start_state])
        while queue:
            current_state = queue.popleft()
            for symbol in symbols:
                if symbol == 'ε':  # Excluimos ε del proceso de construcción del DFA
                    continue
                next_states = self.epsilonClosure(self.move(nfa, current_state, symbol))
                if next_states == '' or next_states == ' ':
                    continue
                if next_states not in seen:
                    queue.append(next_states)
                    seen.add(next_states)
                    terminating = nfa.checkIfAcceptingState(nfa.getStatesByLabel(next_states.split()))
                    dfa_states.setdefault(current_state, {})[symbol] = next_states
                    if terminating:
                        dfa_states.setdefault(next_states, {})['isTerminatingState'] = True
        return dfa_states

# Clase de ejemplo para NFA
class NFA:
    def __init__(self, entrada):
        self.states = []
        self.initial_state = None
        self.accepting_states = set()
        self.alphabet = set()
       
        lineas = entrada.strip().split('\n')

        for linea in lineas:
            if linea.startswith('#states'):
                section = 1
                continue
            if section ==1:
                if linea.startswith('#'):
                    section =2
                    continue
                #GUARDAR ESTADOS
                self.states.append(State(linea))
            if section ==2:
                if linea.startswith('#'):
                    section =3
                    continue
                for i, value in enumerate(self.states):
                    #print(linea, value.label)
                    if linea == value.label:
                        #GUARDAR ESTADO INICIAL
                        self.initial_state = self.states[i]
            if section ==3:
                if linea.startswith('#'):
                    section =4
                    continue
                for i, value in enumerate(self.states):
                    #print(linea, value.label)
                    if linea == value.label:
                        #GUARDAR ESTADO ACEPTACIÓN
                        self.accepting_states.add(self.states[i])
            if section ==4:
                if linea.startswith('#'):
                    section =5
                    continue
                #GUARDAR ALFABETO
                self.alphabet.add(linea)
            if section ==5:
                if linea.startswith('#'):
                    break
                partes = linea.strip().split(':')
                source = partes[0]
                symbol, destination = partes[1].split('>')
                #print(source,symbol,destination)
                for i, value in enumerate(self.states):
                    #self.states[4].transitions = [('b', self.states[5])]
                    if source == value.label:
                        #GUARDAR TRANSICIONES
                        for j, valuee in enumerate(self.states):
                            if destination == valuee.label:
                                self.states[i].transitions.append((symbol, self.states[j]))

    def getStates(self):
        return self.states
    
    def getStateByLabel(self, label):
        for state in self.states:
            if state.label == label:
                return state
        return None
    
    def getStatesByLabel(self, labels):
        states = []
        for label in labels:
            states.append(self.getStateByLabel(label))
        return states
    
    def getSymbols(self):
        return self.alphabet
    
    def checkIfAcceptingState(self, states):
        for state in states:
            if state in self.accepting_states:
                return True
        return False

class State:
    def __init__(self, label):
        self.label = label
        self.transitions = []

def generate_dfa_string(dfa):
    dfa_string = ""
    
    dfa_string += "#states\n"
    for state in dfa.states:
        if state != 'startingState' and not state.endswith('TERMINATINGSTATE'):
            dfa_string += state + "\n"
    
    dfa_string += "#initial\n"
    dfa_string += dfa.states['startingState'] + "\n"
    
    dfa_string += "#accepting\n"
    for state, info in dfa.states.items():
        if 'isTerminatingState' in info:
            dfa_string += state + "\n"
    
    dfa_string += "#alphabet\n"
    for symbol in dfa.getSymbols():
        dfa_string += symbol + "\n"
    
    dfa_string += "#transitions\n"
    for state, transitions in dfa.states.items():
        if state == 'startingState':
            continue
        for symbol, next_state in transitions.items():
            if not isinstance(next_state, bool):  # Excluir entradas tipo 'isTerminatingState'
                dfa_string += f"{state}:{symbol}>{next_state}\n"
    
    return dfa_string

def convertir_formato(cadena):
    lineas = cadena.split('\n')
    
    nuevos_estados = set()
    nuevos_iniciales = set()
    nuevos_aceptados = set()
    nuevas_transiciones = []
    aceptaciones_leidos=False
    
    for linea in lineas:
        if linea.startswith('#states'):
            estados_leidos = True
            continue
        if estados_leidos and not linea.startswith('#initial'):
            estado_acortado = linea.split()[0]
            #print(estado_acortado)
            if estado_acortado not in nuevos_estados:
                nuevos_estados.add(estado_acortado + '\n')  # Agregamos un salto de línea al final del estado
        else:
            break


    for linea in lineas:
        if linea.startswith('#initial'):
            siguiente_linea = lineas[lineas.index(linea) + 1]
            nuevos_iniciales = siguiente_linea.split()[0]
            break
            #print("aqui", nuevos_iniciales)

    for linea in lineas:
        if linea.startswith('#accepting'):
            aceptaciones_leidos = True
            continue
        if aceptaciones_leidos and linea.startswith('#alphabet'):
            break
        elif aceptaciones_leidos and not linea.startswith('#transitions'):
            aceptacion_cortado = linea.split()[0]
            if aceptacion_cortado not in nuevos_aceptados:
                nuevos_aceptados.add(aceptacion_cortado)  # Agregamos un salto de línea al final del estado

    for linea in lineas:
        if linea.startswith('#transitions'):
            transiciones = lineas[lineas.index(linea) + 1:]
            for transicion in transiciones:
                if transicion:
                    partes = transicion.split(':')
                    estado_origen = partes[0]
                    trans = partes[1].split('>')[0]
                    estado_destino = partes[1].split('>')[1]
                    if len(estado_origen.split()) > 1:
                        nuevo_estado_origen = estado_origen.split()[0]
                    else:
                        nuevo_estado_origen = estado_origen
                    if len(estado_destino.split()) > 1:
                        nuevo_estado_destino = estado_destino.split()[0]
                    else:
                        nuevo_estado_destino = estado_destino
                    nuevas_transiciones.append(f"{nuevo_estado_origen}:{trans}>{nuevo_estado_destino}")

    nuevo_formato = ""
    nuevo_formato += "#states\n"
    nuevo_formato += "".join(nuevos_estados)  # Unimos los estados sin espacios entre ellos
    nuevo_formato += "#initial\n"
    nuevo_formato += ""+nuevos_iniciales+ "\n"
    nuevo_formato += "#accepting\n"
    nuevo_formato += " ".join(nuevos_aceptados) + "\n"
    nuevo_formato += "#alphabet\n"
    nuevo_formato += "a\nb\n"
    nuevo_formato += "#transitions\n"
    nuevo_formato += "\n".join(nuevas_transiciones)

    return nuevo_formato

def main():
    #if len(sys.argv) != 2:
        #print("Usage: python3 NFAtoDFA.py <input_string>")
        #return
    
    input_string = sys.argv[1]
    #print("Input string received:", input_string)

    
    nfa = NFA(input_string)
    dfa = DFA(nfa)
    
    dfa_string = generate_dfa_string(dfa)
    #print(dfa_string)
    print(convertir_formato(dfa_string))


if __name__ == "__main__":
    main()
