#include "calculator.h"         // Bindet die Deklaration der Funktion `calculate` und auch den darin enthaltenden `ParsedExpression`-Typ ein

long long calculate(const ParsedExpression* expr, long long scale) {
// Führt die mathemische Berechnung entsprechend des Operators aus
// - `expr` ist ein Zeiger auf den geparsten Ausdruck (mit operand1, operand2, operator)
// - `scale` ist der Skalierungsfaktor für Festkommazahlen (z. B. 100 für zwei Nachkommastellen)

  switch (expr->op) {           // Auswahl basierend auf dem Operator im Ausdruck

    // Addition zweier Zahlen
    case '+':
      return expr->operand1 + expr->operand2;

    // Subtraktion zweier Zahlen
    case '-': 
      return expr->operand1 - expr->operand2;

      // Multiplikation mit Rückskalierung auf Festkommaniveau
    case '*': 
      return (expr->operand1 * expr->operand2 + scale / 2) / scale;         // `(a * b + scale/2) / scale` -> Rundung auf das Ergebnis in fester Kommazahl

      // Division mit Rückskalierung und Rundung
    case '/': 
      return (expr->operand2 != 0) 
        ? ((expr->operand1 * scale + expr->operand2 / 2) / expr->operand2) 
        : 0;                                                                // Schutz gegen Division durch 0 –> Es wird 0 zurückgegeben


    default: 
      return 0;                 // Falls der Operator nicht erkannt wird, gibt die Funktion 0 zurück
  }
}