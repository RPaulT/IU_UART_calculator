#ifndef PARSER_H                // Prüft, ob PARSER_H noch nicht definiert wurde (Include Guard)
#define PARSER_H                // Definiert PARSER_H

#include <stdbool.h>            // Ermöglicht die Verwendung des `bool`-Datentyps in C

#ifdef __cplusplus              // Wenn der Code von einem C++-Compiler kompiliert wird
extern "C" {                    // Verhindert C++-Namensmangling und erlaubt Kompilierung mit C++-Linker
#endif

typedef struct {                // Definition eines Strukturs namens ParsedExpression
  long long operand1;           // Erster Operand (z. B. die Zahl links vom Operator)
  long long operand2;           // Zweiter Operand (z. B. die Zahl rechts vom Operator)
  char op;                      // Operator (z. B. '+', '-', '*', '/')
} ParsedExpression;

// Deklaration der Funktion `parseExpression`, die einen Rechenausdruck als String verarbeitet,
// das Ergebnis in `result` speichert, und `true` bei erfolgreichem Parsen zurückgibt.
// `scale` ist der Festkomma-Skalierungsfaktor (standardmäßig 100 [10^2] -> zwei Nachkommastellen)
bool parseExpression(const char* input, ParsedExpression* result, long long scale);

#ifdef __cplusplus              // Falls vorher mit extern "C" geöffnet, hier wieder schließen
}
#endif

#endif                          // Ende des Include Guards