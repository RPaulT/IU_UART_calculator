#include "parser.h"
#include <ctype.h>
#include <string.h>

long long parseDecimalToFixed(const char* str, long long scale) {
  long long result = 0;
  long long frac = 0;
  int fracDigits = 0;
  bool pastDecimal = false;

  // Parsen der Ganz- und Nachkommastellen
  while (*str) {
    if (*str == '.') {
      pastDecimal = true;
    } else if (isdigit(*str)) {
      if (pastDecimal) {
        if (fracDigits < 18) {
          frac = frac * 10 + (*str - '0');
          fracDigits++;
        }
      } else {
        result = result * 10 + (*str - '0');
      }
    }
    str++;
  }

  // Skaliere auf das eingestellte scale
  long long factor = 1;
  for (int i = 0; i < fracDigits; i++) factor *= 10;

  while (factor < scale) {
    frac *= 10;
    factor *= 10;
  }

  // Kaufmännisches Runden auf scale-Niveau
  long long scaledFrac = (frac * scale + factor / 2) / factor;

  return result * scale + scaledFrac;
}

bool parseExpression(const char* input, ParsedExpression* result, long long scale) {
  char buf[64];
  int j = 0;

  for (int i = 0; input[i] != '\0' && j < sizeof(buf) - 1; i++) {
    if (!isspace(input[i])) {
      buf[j++] = (input[i] == ',') ? '.' : input[i];
    }
  }
  buf[j] = '\0';

  const char* ops = "+-*/";
  char* op_ptr = NULL;
  for (int i = 0; ops[i]; i++) {
    op_ptr = strchr(buf, ops[i]);
    if (op_ptr) {
      result->op = ops[i];
      break;
    }
  }

  if (!op_ptr || op_ptr == buf || *(op_ptr + 1) == '\0') return false;

  char left[32], right[32];
  strncpy(left, buf, op_ptr - buf);
  left[op_ptr - buf] = '\0';
  strncpy(right, op_ptr + 1, sizeof(right) - 1);
  right[sizeof(right) - 1] = '\0';

  result->operand1 = parseDecimalToFixed(left, scale);
  result->operand2 = parseDecimalToFixed(right, scale);
  return true;
}