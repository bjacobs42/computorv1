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

  ast::ExprPtr parse(void);

private:
  using NudFn = ast::ExprPtr (Parser::*)(void);
  using LedFn = ast::ExprPtr (Parser::*)(ast::ExprPtr, BindingPower);

  struct Entry
  {
    BindingPower bp = BindingPower::default_bp;
    NudFn nud = NULL;
    LedFn led = NULL;
  };

  std::array<Entry, (int)TokenType::END + 1> _lookup;
  std::vector<Token> _tokens;
  size_t _pos;

  // navigation
  inline bool _has_tokens(void) const;
  inline const Token &_peek(void) const;
  inline TokenType _peek_type(void) const;
  const Token &_consume(void);
  TokenType _look_ahead(void) const;

  // handlers
  ast::ExprPtr _parse_expr(BindingPower bp);
  ast::ExprPtr _parse_number_expr(void);
  ast::ExprPtr _parse_binary_expr(ast::ExprPtr left, BindingPower bp);
  ast::ExprPtr _parse_implicit_expr(ast::ExprPtr left, BindingPower bp);
  ast::ExprPtr _parse_power_expr(ast::ExprPtr left, BindingPower bp);

  // registration
  void _register_nud(TokenType token_type, BindingPower bp, NudFn nud);
  void _register_led(TokenType token_type, BindingPower bp, LedFn led);
  void _init_lookup(void);
};
