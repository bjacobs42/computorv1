#pragma once

#include "Token.hpp"
#include <string>
#include <vector>

enum class CharClass
{
  digit,
  alpha,
  oper,
  unknown
};

class Lexer
{
public:
  Lexer(const std::string &input);
  ~Lexer(void);

  std::vector<Token> lex(void);

private:
  size_t _pos;
  const std::string &_input;

  CharClass _classify(char character);

  Token _handle_number(void);
  Token _handle_variable(void);
  Token _handle_operator(void);
  Token _handle_error(void);

  using lex_handler = Token (Lexer::*)(void);

  static constexpr lex_handler _token_handlers[] = {
      &Lexer::_handle_number,
      &Lexer::_handle_variable,
      &Lexer::_handle_operator,
      &Lexer::_handle_error
  };
};
