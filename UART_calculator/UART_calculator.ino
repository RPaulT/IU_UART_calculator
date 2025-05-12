#include "parser.h"
#include "calculator.h"

String inputBuffer = "";
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
  if (precision == 0) {
    Serial.println();
    return;
  }

  Serial.print(".");

  long long pad = scale / 10;
  while (pad > 1 && fractionalPart < pad) {
    Serial.print("0");
    pad /= 10;
  }

  for (int i = precision - 1; i >= 0; i--) {
    long long divisor = 1;
    for (int j = 0; j < i; j++) divisor *= 10;
    int digit = (fractionalPart / divisor) % 10;
    Serial.print(digit);
  }

  Serial.println();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputBuffer.trim();
      ParsedExpression expr;
      if (parseExpression(inputBuffer.c_str(), &expr, scale)) {
        if (expr.op == '/' && expr.operand2 == 0) {
          Serial.println("Fehler: Division durch 0 nicht möglich.");
        } else {
          long long result = calculate(&expr, scale);

          Serial.print("Ergebnis: ");
          printFixedPoint(result, scale, precision);
        }
      } else {
        Serial.println("Fehlerhafte Eingabe. Bitte Eingabe im Format: Zahl Operator Zahl.");
      }
      inputBuffer = "";
    } else {
      inputBuffer += c;
    }
  }
}