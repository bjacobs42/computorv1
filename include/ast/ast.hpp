#pragma once

#include "lexer/Token.hpp"
#include <memory>

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

  using ExprPtr = std::unique_ptr<Expr>;

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
    BinaryExpr(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)), op(op), right(std::move(right))
    {
      kind = ExprKind::binary;
    }

    ExprPtr left;
    Token op;
    ExprPtr right;
  };

  template <typename T, typename... Args> ExprPtr new_expr(Args &&...args)
  {
    return (std::make_unique<T>(std::forward<Args>(args)...));
  }
} // namespace ast
