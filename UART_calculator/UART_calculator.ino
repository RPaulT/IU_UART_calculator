#include "parser.h"          // Enthält Definitionen zum Parsen von Rechenausdrücken
#include "calculator.h"      // Enthält Funktion zur Berechnung des Ergebnisses
#include <string.h>          // Für Stringfunktionen wie z.B. strncmp()
#include <stdbool.h>         // Für den bool-Typ in C

char inputBuffer[64];        // Puffer zum Zwischenspeichern der seriellen Eingabe
int bufferIndex = 0;         // Position im Eingabepuffer

int precision = 2;           // Anzahl der Nachkommastellen, initial 2
long long scale = 100;       // Skalierungsfaktor für Festkomma-Rechnung (10^precision [Starteinstellung also 10^2])

void setup() {
  Serial.begin(9600);        // Serielle Kommunikation mit 9600 Baud starten
  Serial.println("Bereit für Eingaben (z. B. 1.53+1.005 oder 4,5 * 2):");
}

void printFixedPoint(long long value, long long scale, int precision) {
  if (value < 0) {           // Vorzeichenbehandlung
    Serial.print("-");
    value = -value;
  }

  long integerPart = value / scale;            // Ganzzahliger Anteil
  long long fractionalPart = value % scale;    // Nachkommaanteil
  Serial.print(integerPart);                   // Ganzzahligen Teil ausgeben

  if (precision == 0 || fractionalPart == 0) { // Falls keine Nachkommastellen nötig
    Serial.println();
    return;
  }

  Serial.print(",");                           // Dezimalkomma ausgeben
  char digits[16];                             // Puffer für Nachkommastellen
  int count = 0;
  long long div = scale / 10;                  // Startdivisor für erste Nachkommastelle

  while (div > 0 && count < precision) {
    int digit = (fractionalPart / div) % 10;   // Einzelne Ziffer berechnen
    digits[count++] = '0' + digit;             // In Zeichen umwandeln
    div /= 10;                                 // Nächste Stelle
  }

  while (count > 0 && digits[count - 1] == '0') {
    count--;                                   // Überflüssige Nullen am Ende kürzen
  }

  for (int i = 0; i < count; i++) {
    Serial.print(digits[i]);                   // Restliche Ziffern ausgeben
  }
  Serial.println();                            // Zeilenumbruch nach Zahl
}

void loop() {
  while (Serial.available()) {                 // Wenn serielle Eingabe vorhanden
    char c = Serial.read();                    // Zeichen lesen

    if (c == '\n' || c == '\r') {              // Bei Zeilenende
      inputBuffer[bufferIndex] = '\0';         // Eingabe abschließen
      bufferIndex = 0;                         // Puffer zurücksetzen

      // Prüfen ob Präzisionsbefehl eingegeben wurde
      if (strncmp(inputBuffer, "SETPREC ", 8) == 0) {
        int newPrecision = atoi(&inputBuffer[8]);       // neue Präzision extrahieren
        if (newPrecision >= 0 && newPrecision <= 10) {
          precision = newPrecision;                     // neue Präzision setzen

          scale = 1;                                    // neue Skalierung berechnen
          for (int i = 0; i < precision; i++) scale *= 10;

          Serial.print("Anzahl der Nachkommastellen: ");
          Serial.println(precision);
        } else {
          Serial.println("Fehlerhafte Eingabe der Nachkommastellen. Bereich: 0–10. (z.B. 'SETPREC 4')");
        }
        return;                                         // keine Rechenverarbeitung mehr
      }

      // Normale Rechenverarbeitung
      ParsedExpression expr;                            // Ausdrucksstruktur
      if (parseExpression(inputBuffer, &expr, scale)) { // Ausdruck parsen
        if (expr.op == '/' && expr.operand2 == 0) {
          Serial.println("Fehler: Division durch 0");   // Sonderfall: Division durch 0
        } else {
          long long result = calculate(&expr, scale);   // Ergebnis berechnen
          Serial.print("Ergebnis: ");
          printFixedPoint(result, scale, precision);    // Ergebnis ausgeben
        }
      } else {
        Serial.println("Fehlerhafte Eingabe. Bitte Eingabe im Format: Zahl Operator Zahl.");
      }

    } else if (bufferIndex < sizeof(inputBuffer) - 1) {
      inputBuffer[bufferIndex++] = c;                  // Zeichen in Eingabepuffer speichern
    }
  }
}