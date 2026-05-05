#pragma once

#include "ast/ast.hpp"
#include "lexer/Token.hpp"
#include <array>
#include <vector>

enum class BindingPower
{
  default_bp = 0,
  additive = 5,
  multiplicative = 10,
  exponential = 15,
  number = 20
};

class Parser
{
public:
  explicit Parser(const std::string &input);
  ~Parser(void);

  ast::Expr parse(void);

private:
  using NudFn = ast::Expr (Parser::*)(void);
  using LedFn = ast::Expr (Parser::*)(ast::Expr, BindingPower);

  struct Entry
  {
    BindingPower bp;
    NudFn nud = NULL;
    LedFn led = NULL;
  };

  std::array<Entry, (int)TokenType::END> _lookup;
  std::vector<Token> _tokens;
  size_t _pos;

  // navigation
  inline bool _has_tokens(void) const;
  inline Token _peek(void) const;
  inline TokenType _peek_type(void) const;
  Token _consume(void);

  // handlers
  ast::Expr _parse_expression(BindingPower bp);
  ast::Expr _parse_number_expression(void);
  ast::Expr _parse_binary_expression(ast::Expr left, BindingPower bp);

  // registration
  void _register_nud(TokenType token_type, BindingPower bp, NudFn nud);
  void _register_led(TokenType token_type, BindingPower bp, LedFn led);
  void _init_lookup(void);
};
