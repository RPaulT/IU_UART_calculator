#ifndef CALCULATOR_H            // Prüft, ob CALCULATOR_H noch nicht definiert wurde (Include Guard)
#define CALCULATOR_H            // Definiert CALCULATOR_H

#include "parser.h"             // Bezieht die Definition des Typs ParsedExpression aus parser.h

#ifdef __cplusplus              // Wenn der Code von einem C++-Compiler kompiliert wird
extern "C" {                    // Verhindert C++-Namensmangling und erlaubt Kompilierung mit C++-Linker
#endif

// Deklaration der Funktion `calculate`, die einen ParsedExpression und ein Skalierungsfaktor bekommt
// und das Ergebnis als Festkommazahl (long long) zurückgibt
long long calculate(const ParsedExpression* expr, long long scale);

#ifdef __cplusplus              // Falls vorher mit extern "C" geöffnet, hier wieder schließen
}
#endif

#endif                          // Ende des Include Guards