#include "calculator.h"

long long calculate(const ParsedExpression* expr, long long scale) {
  switch (expr->op) {
    case '+': return expr->operand1 + expr->operand2;
    case '-': return expr->operand1 - expr->operand2;
    case '*': return (expr->operand1 * expr->operand2 + scale / 2) / scale;
    case '/': return (expr->operand2 != 0) ? ((expr->operand1 * scale + expr->operand2 / 2) / expr->operand2) : 0;
    default: return 0;
  }
}