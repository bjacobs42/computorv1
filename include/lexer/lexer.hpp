#pragma once

#include "token.hpp"
#include <string>
#include <vector>

enum class char_class
{
  digit,
  alpha,
  oper,
  unknown
};

class lexer
{
public:
  lexer(const std::string &input);
  ~lexer(void);

  std::vector<token> lex(void);

private:
  size_t _pos;
  const std::string &_input;

  char_class _classify(char character);

  token _handle_number(void);
  token _handle_variable(void);
  token _handle_operator(void);
  token _handle_error(void);

  using lex_handler = token (lexer::*)(void);

  static constexpr lex_handler _token_handlers[] = {
      &lexer::_handle_number,
      &lexer::_handle_variable,
      &lexer::_handle_operator,
      &lexer::_handle_error
  };
};
