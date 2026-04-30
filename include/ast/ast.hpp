#pragma once

#include "lexer/Token.hpp"

namespace ast
{
  enum class ExprKind
  {
    number,
    variable,
    binary
  };

  struct Expr
  {
    ExprKind kind;
    virtual ~Expr(void) = default;
  };

  struct NumberExpr : Expr
  {
    NumberExpr(void) { kind = ExprKind::number; };
    NumberExpr(float init_value)
    {
      value = init_value;
      kind = ExprKind::number;
    };
    float value;
  };

  struct VariableExpr : Expr
  {
    VariableExpr(void) { kind = ExprKind::variable; };
    VariableExpr(char variable_name)
    {
      name = variable_name;
      kind = ExprKind::variable;
    };
    char name;
  };

  struct BinaryExpr
  {
    Expr left;
    Token op;
    Expr right;
  };
} // namespace ast
