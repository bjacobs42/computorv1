#pragma once

#include "lexer/token.hpp"

namespace ast
{
  enum class expr_kind
  {
    number,
    variable,
    binary
  };

  struct expr
  {
    expr_kind kind;
    virtual ~expr(void) = default;
  };

  struct number_expr : expr
  {
    number_expr(void) { kind = expr_kind::number; };
    number_expr(float init_value)
    {
      value = init_value;
      kind = expr_kind::number;
    };
    float value;
  };

  struct variable_expr : expr
  {
    variable_expr(void) { kind = expr_kind::variable; };
    variable_expr(char variable_name)
    {
      name = variable_name;
      kind = expr_kind::variable;
    };
    char name;
  };

  struct binary_expr
  {
    expr left;
    token op;
    expr right;
  };
} // namespace ast
