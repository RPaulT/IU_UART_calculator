#include "parser.h"             // Enthält Definitionen für ParsedExpression und parseExpression()
#include <ctype.h>              // Für isdigit() und isspace()
#include <string.h>             // Für Stringfunktionen wie strncpy(), strspn()
#include <stdbool.h>            // Für den bool-Typ in C

// Wandelt eine Zahl im Stringformat in eine Festkommazahl (kaufmännisches Runden)
long long parseDecimalToFixed(const char* str, long long scale) {
    long long result = 0;       // Ganzzahliger Teil
    long long frac = 0;         // Nachkommateil (als Ganzzahl dargestellt)
    int fracDigits = 0;         // Anzahl der Nachkommastellen im String
    bool pastDecimal = false;   // Flag, ob ein Dezimalpunkt gefunden wurde
    bool negative = false;      // Flag für negatives Vorzeichen

    if (*str == '-') {                                // Prüfe auf Vorzeichen vor erster Ziffer
        negative = true;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str) {                                    // Zeichenweise durch die Zahl iterieren
        if (*str == '.') {
            pastDecimal = true;
        } else if (isdigit((unsigned char)*str)) {
            if (pastDecimal) {
                if (fracDigits < 18) {                // Maximal 18 Stellen für Nachkomma beachten
                    frac = frac * 10 + (*str - '0');
                    fracDigits++;
                }
            } else {
                result = result * 10 + (*str - '0');  // Ganzzahl aufbauen
            }
        } else {
            return 0;                                 // Ungültiges Zeichen -> Fehler
        }
        str++;
    }

    // Skaliere den Nachkommateil auf das gewünschte scale-Niveau
    long long factor = 1;
    for (int i = 0; i < fracDigits; i++) factor *= 10;

    while (factor < scale) {
        frac *= 10;
        factor *= 10;
    }

    // Runden und zusammensetzen
    long long scaledFrac = (frac * scale + factor / 2) / factor;
    long long total = result * scale + scaledFrac;

    return negative ? -total : total;                 // Falls negativ, Ergebnis negieren
}

// Hilfsfunktion: Zählt die Anzahl der gültigen Operatoren im String
int countOperators(const char* str) {
    int count = 0;
    for (int i = 1; str[i] != '\0'; i++) {
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
            count++;                                  // Nur zählen, wenn der Operator *nicht* am Anfang steht
        }
    }
    return count;
}

// Hauptfunktion: Teilt den Eingabestring in Operand1, Operator und Operand2 auf
bool parseExpression(const char* input, ParsedExpression* result, long long scale) {
    char cleaned[64];                                 // Aufbereitete Eingabe ohne Leerzeichen und mit '.' statt ','
    int j = 0;

    // Leerzeichen entfernen, Kommas zu Punkten konvertieren
    for (int i = 0; input[i] != '\0' && j < 63; i++) {
        if (!isspace((unsigned char)input[i])) {
            cleaned[j++] = (input[i] == ',') ? '.' : input[i];
        }
    }
    cleaned[j] = '\0';
    // Zeichenprüfung: Nur gültige Zeichen erlaubt
    for (int i = 0; cleaned[i] != '\0'; i++) {
    char c = cleaned[i];
    if (!(isdigit((unsigned char)c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '.')) {
        return false;                                 // Ungültiges Zeichen wie z.B. ;
    }
}

    // Es darf nur genau ein Operator vorkommen (den Operator vor der ersten Zahl ausgeschlossen)
    if (countOperators(cleaned) != 1) return false;

    // Position des Operators suchen
    int opIndex = -1;
    for (int i = 1; cleaned[i] != '\0'; i++) {
        if (cleaned[i] == '+' || cleaned[i] == '-' || cleaned[i] == '*' || cleaned[i] == '/') {
            opIndex = i;
            result->op = cleaned[i];
            break;
        }
    }

    // Ungültig, wenn Operator ganz am Anfang oder kein rechter Operand
    if (opIndex <= 0 || cleaned[opIndex + 1] == '\0') return false;

    // Eingabe in linken und rechten Teil aufspalten
    char left[32], right[32];
    strncpy(left, cleaned, opIndex);
    left[opIndex] = '\0';
    strncpy(right, cleaned + opIndex + 1, sizeof(right) - 1);
    right[sizeof(right) - 1] = '\0';

    // Prüfen, ob beide Seiten Ziffern enthalten (zur Sicherheit)
    if (strspn(left, "-+0123456789.") == 0 || strspn(right, "-+0123456789.") == 0)
        return false;

    // Operanden umwandeln in Festkommazahlen
    result->operand1 = parseDecimalToFixed(left, scale);
    result->operand2 = parseDecimalToFixed(right, scale);

    return true; // Erfolgreich geparst
}
