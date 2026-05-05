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

  struct BinaryExpr : Expr
  {
    BinaryExpr(void) { kind = ExprKind::binary; };
    BinaryExpr(Expr *new_left, Token *new_op, Expr *new_right)
    {
      kind = ExprKind::binary;
      left = new_left;
      op = new_op;
      right = new_right;
    };

    Expr *left;
    Token *op;
    Expr *right;
  };
} // namespace ast
