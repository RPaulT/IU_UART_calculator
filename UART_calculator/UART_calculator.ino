#include "parser.h"
#include "calculator.h"
#include <string.h>
#include <stdbool.h>

char inputBuffer[64];
int bufferIndex = 0;

int precision = 2;
long long scale = 100;

void setup() {
  Serial.begin(9600);
  Serial.println("Bereit für Eingaben (z. B. 1.53+1.005 oder 4,5 * 2):");
}

void printFixedPoint(long long value, long long scale, int precision) {
  if (value < 0) {
    Serial.print("-");
    value = -value;
  }

  long integerPart = value / scale;
  long long fractionalPart = value % scale;

  Serial.print(integerPart);

  if (precision == 0 || fractionalPart == 0) {
    Serial.println();
    return;
  }

  Serial.print(".");

  // Ziffern vorbereiten
  char digits[16];  // maximal 15 Nachkommastellen
  int count = 0;

  long long div = scale / 10;
  while (div > 0 && count < precision) {
    int digit = (fractionalPart / div) % 10;
    digits[count++] = '0' + digit;
    div /= 10;
  }

  // Überflüssige Nullen am Ende abschneiden
  while (count > 0 && digits[count - 1] == '0') {
    count--;
  }

  // Ausgabe
  for (int i = 0; i < count; i++) {
    Serial.print(digits[i]);
  }

  Serial.println();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      inputBuffer[bufferIndex] = '\0';
      bufferIndex = 0;

      // Befehl zur Änderung der Präzision erkennen
      if (strncmp(inputBuffer, "SETPREC ", 8) == 0) {
        int newPrecision = atoi(&inputBuffer[8]);
        if (newPrecision >= 0 && newPrecision <= 10) {
          precision = newPrecision;

          // Neue Skalierung berechnen
          scale = 1;
          for (int i = 0; i < precision; i++) scale *= 10;

          Serial.print("Anzahl der Nachkommastellen: ");
          Serial.println(precision);
        } else {
          Serial.println("Fehlerhafte Eingabe der Nachkommastellen. Bereich: 0–10. (z.B. 'SETPREC 4')");
        }
        return;
      }

      // Rechenmodus wie bisher
      ParsedExpression expr;
      if (parseExpression(inputBuffer, &expr, scale)) {
        if (expr.op == '/' && expr.operand2 == 0) {
          Serial.println("Fehler: Division durch 0");
        } else {
          long long result = calculate(&expr, scale);
          Serial.print("Ergebnis: ");
          printFixedPoint(result, scale, precision);
        }
      } else {
        Serial.println("Fehlerhafte Eingabe. Bitte Eingabe im Format: Zahl Operator Zahl.");
      }
    }
    else if (bufferIndex < sizeof(inputBuffer) - 1) {
      inputBuffer[bufferIndex++] = c;
    }
  }
}