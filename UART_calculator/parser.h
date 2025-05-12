#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

typedef struct {
  long long operand1;
  long long operand2;
  char op;
} ParsedExpression;

bool parseExpression(const char* input, ParsedExpression* result, long long scale);

#endif