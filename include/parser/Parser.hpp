#pragma once

#include "ast/ast.hpp"
#include "lexer/Token.hpp"
#include <vector>

class Parser
{
public:
  explicit Parser(const std::string &input);
  ~Parser(void);

  ast::Expr parse(void);

private:
  std::vector<Token> _tokens;
  size_t _pos;

  inline Token _peek(void) const;
  Token _consume(void);

  inline bool _has_tokens(void) const;
  inline TokenType _peek_type(void) const;
};
