#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT 256

// ----------------- SCAN -----------------
bool scan(const char *cadena, const char *caracteresValidos, char tokens[][2], int *tokenCount) {
    *tokenCount = 0;
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (strchr(caracteresValidos, cadena[i]) != NULL) {
            tokens[*tokenCount][0] = cadena[i];
            tokens[*tokenCount][1] = '\0';
            (*tokenCount)++;
        } else {
            printf("Error: caracter ingresado invalido: '%c'. Caracteres válidos: %s\n", cadena[i], caracteresValidos);
            return false;
        }
    }
    return true;
}

// ----------------- GRAMÁTICAS -----------------

// G1: Palíndromos de 0s y 1s con longitud impar
bool parse_grammar_1(char tokens[][2], int n) {
    if (n == 0 || n % 2 == 0) return false;
    for (int i = 0; i < n / 2; i++) {
        if (tokens[i][0] != tokens[n - i - 1][0]) return false;
    }
    return true;
}

// G2: A -> ε | a A b ; S -> A b
int index2;
bool match2(char tokens[][2], int n, char expected) {
    if (index2 < n && tokens[index2][0] == expected) {
        index2++;
        return true;
    }
    return false;
}
bool parse_A2(char tokens[][2], int n) {
    if (index2 >= n) return true;
    if (match2(tokens, n, 'a')) {
        if (parse_A2(tokens, n)) {
            if (match2(tokens, n, 'b')) return true;
        }
        return false;
    }
    return true;
}
bool parse_grammar_2(char tokens[][2], int n) {
    index2 = 0;
    bool result = parse_A2(tokens, n);
    if (result && match2(tokens, n, 'b') && index2 == n) return true;
    return false;
}

// G3: a^n b^m donde n>=1, m>=1
bool parse_grammar_3(char tokens[][2], int n) {
    if (n < 2) return false;
    int i = 0, a_count = 0, b_count = 0;
    while (i < n && tokens[i][0] == 'a') { a_count++; i++; }
    if (a_count == 0) return false;
    while (i < n && tokens[i][0] == 'b') { b_count++; i++; }
    return (b_count > 0 && i == n);
}

// G4: ab o abb
bool parse_grammar_4(char tokens[][2], int n) {
    if (n == 2) return (tokens[0][0] == 'a' && tokens[1][0] == 'b');
    if (n == 3) return (tokens[0][0] == 'a' && tokens[1][0] == 'b' && tokens[2][0] == 'b');
    return false;
}

// G5
int index5;
bool match5(char tokens[][2], int n, char expected) {
    if (index5 < n && tokens[index5][0] == expected) {
        index5++;
        return true;
    }
    return false;
}
bool parse_A5(char tokens[][2], int n) {
    int start = index5;
    if (match5(tokens, n, 'a')) return true;
    index5 = start;
    if (match5(tokens, n, 'b')) {
        if (parse_A5(tokens, n)) {
            if (match5(tokens, n, 'a') && match5(tokens, n, 'b')) return true;
        }
    }
    index5 = start;
    return false;
}
bool parse_S5(char tokens[][2], int n) {
    int start = index5;
    if (parse_A5(tokens, n) && match5(tokens, n, 'b')) return true;
    index5 = start;
    return false;
}
bool parse_grammar_5(char tokens[][2], int n) {
    index5 = 0;
    bool result = parse_S5(tokens, n);
    return (result && index5 == n);
}

// ----------------- MAIN -----------------
int main() {
    char input[MAX_INPUT];
    while (1) {
        printf("Gramáticas disponibles: G1, G2, G3, G4, G5\n");
        printf("'salir' para terminar el programa\n\n");

        printf("Elige la gramática a usar (ej: G2): ");
        if (!fgets(input, MAX_INPUT, stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (strcasecmp(input, "salir") == 0) break;

        char grammar[3];
        strncpy(grammar, input, 3);

        printf("Ingresa la cadena a analizar: ");
        if (!fgets(input, MAX_INPUT, stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcasecmp(input, "volver") == 0) continue;

        char tokens[MAX_INPUT][2];
        int tokenCount = 0;
        bool scanned = false;

        if (strcasecmp(grammar, "G1") == 0) scanned = scan(input, "01", tokens, &tokenCount);
        else if (strcasecmp(grammar, "G2") == 0) scanned = scan(input, "ab", tokens, &tokenCount);
        else if (strcasecmp(grammar, "G3") == 0) scanned = scan(input, "ab", tokens, &tokenCount);
        else if (strcasecmp(grammar, "G4") == 0) scanned = scan(input, "ab", tokens, &tokenCount);
        else if (strcasecmp(grammar, "G5") == 0) scanned = scan(input, "ab", tokens, &tokenCount);
        else {
            printf("Error: Gramática no reconocida.\n");
            continue;
        }

        if (!scanned) continue;

        bool is_valid = false;
        if (strcasecmp(grammar, "G1") == 0) is_valid = parse_grammar_1(tokens, tokenCount);
        else if (strcasecmp(grammar, "G2") == 0) is_valid = parse_grammar_2(tokens, tokenCount);
        else if (strcasecmp(grammar, "G3") == 0) is_valid = parse_grammar_3(tokens, tokenCount);
        else if (strcasecmp(grammar, "G4") == 0) is_valid = parse_grammar_4(tokens, tokenCount);
        else if (strcasecmp(grammar, "G5") == 0) is_valid = parse_grammar_5(tokens, tokenCount);

        printf("\n------------------------------\n");
        if (is_valid) {
            printf(" La cadena '%s' es VÁLIDA para %s.\n", input, grammar);
        } else {
            printf(" La cadena '%s' es INVÁLIDA para %s.\n", input, grammar);
        }
        printf("------------------------------\n\n");
    }
    return 0;
}
