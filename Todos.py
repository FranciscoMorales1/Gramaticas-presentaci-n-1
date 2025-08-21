def scan(CadenaEntrada, CaracteresValidos):
    Tokens = []
    for char in CadenaEntrada:
        if char in CaracteresValidos:
            Tokens.append(char)
        else:
            raise ValueError(f"Error, caracter ingresado invalido: '{char}'. Caracteres válidos: {list(CaracteresValidos)}")
    return Tokens

def parse_grammar_1(tokens):
    """Gramática para palíndromos de 0s y 1s"""
    def is_palindrome(tokens):
        return tokens == tokens[::-1]
    
    if len(tokens) == 0:
        return False
    
    # Verificar si es palíndromo y contiene solo 0s y 1s
    return is_palindrome(tokens) and len(tokens) % 2 == 1

def parse_grammar_2(tokens):
    index = 0
    n = len(tokens)
    
    def match(expected):
        nonlocal index
        if index < n and tokens[index] == expected:
            index += 1
            return True
        return False
    
    def parse_A():
        nonlocal index
        if index >= n:
            return True
            
        if match('a'):
            if parse_A():
                if match('b'):
                    return True
            return False
        return True
    
    try:
        result = parse_A()
        if result and match('b') and index == n:
            return True
        return False
    except:
        return False

def parse_grammar_3(tokens):
    """Gramática: a^n b^m donde n≥1, m≥1"""
    if len(tokens) < 2:
        return False
    
    # Contar 'a's al inicio
    a_count = 0
    i = 0
    while i < len(tokens) and tokens[i] == 'a':
        a_count += 1
        i += 1
    
    # Debe haber al menos una 'a'
    if a_count == 0:
        return False
    
    # Contar 'b's al final
    b_count = 0
    while i < len(tokens) and tokens[i] == 'b':
        b_count += 1
        i += 1
    
    # Debe haber al menos una 'b' y no deben quedar tokens
    return b_count > 0 and i == len(tokens)

def parse_grammar_4(tokens):
    n = len(tokens)
    if n == 2:
        return tokens[0] == 'a' and tokens[1] == 'b'
    elif n == 3:
        return tokens[0] == 'a' and tokens[1] == 'b' and tokens[2] == 'b'
    return False

def parse_grammar_5(tokens):
    """Gramática corregida para G5"""
    index = 0
    n = len(tokens)
    
    def match(expected):
        nonlocal index
        if index < n and tokens[index] == expected:
            index += 1
            return True
        return False
    
    def parse_A():
        nonlocal index
        start_index = index
        
        # Opción 1: A -> a
        if match('a'):
            return True
        
        # Opción 2: A -> bAab
        index = start_index
        if match('b'):
            if parse_A():
                if match('a') and match('b'):
                    return True
        
        index = start_index
        return False
    
    def parse_S():
        nonlocal index
        start_index = index
        
        # S -> Ab
        if parse_A() and match('b'):
            return True
        
        index = start_index
        return False
    
    try:
        index = 0
        result = parse_S()
        return result and index == n
    except:
        return False

grammar_config = {
    "G1": {"valid_chars": "01", "parser_function": parse_grammar_1},
    "G2": {"valid_chars": "ab", "parser_function": parse_grammar_2},
    "G3": {"valid_chars": "ab", "parser_function": parse_grammar_3},
    "G4": {"valid_chars": "ab", "parser_function": parse_grammar_4},
    "G5": {"valid_chars": "ab", "parser_function": parse_grammar_5}
}

if __name__ == "__main__":
    while True:
        print("Gramáticas disponibles: G1, G2, G3, G4, G5")
        print("'salir' para terminar el programa")
        
        chosen_grammar = input("\nElige la gramática a usar (ej: G2): ").strip().upper()

        if chosen_grammar == "SALIR":
            break

        if chosen_grammar not in grammar_config:
            print("Error: Gramática no reconocida. Por favor elige entre G1, G2, G3, G4, G5")
            continue

        config = grammar_config[chosen_grammar]
        input_string = input("Ingresa la cadena a analizar: ").strip()

        if input_string.lower() == "volver":
            continue

        try:
            tokens = scan(input_string, config["valid_chars"])
            print("Tokens:", tokens)

            is_valid = config["parser_function"](tokens)

            print("\n" + "-"*30)
            if is_valid:
                print(f" La cadena '{input_string}' es VÁLIDA para {chosen_grammar}.")
            else:
                print(f" La cadena '{input_string}' es INVÁLIDA para {chosen_grammar}.")
            print("-"*30)

        except ValueError as e:
            print(f" Error durante el escaneo: {e}")
        except Exception as e:
            print(f" Ocurrió un error inesperado: {e}")
        
        input("\nPresiona Enter para continuar...")
