#pragma once

#include "ast/ast.hpp"
#include "lexer/token.hpp"
#include <vector>

class parser
{
public:
  explicit parser(const std::string &input);
  ~parser(void);

  ast::expr parse(void);

private:
  std::vector<token> _tokens;
  size_t _pos;

  inline token _peek(void) const;
  token _consume(void);

  inline bool _has_tokens(void) const;
  inline token_type _peek_type(void) const;
};
