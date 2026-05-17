#include "lexer/Token.hpp"
#include "lexer/TokenError.hpp"
#include <cctype>
#include <string>

Token::~Token(void) {}

Token::Token(const std::string &value, int pos) : _location(pos)
{
  if (value.empty())
    throw TokenError("Empty value", 0, pos);

  _type = Token::get_type(value[0]);
  _value = value;
}

Token::Token(const std::string &value, int pos, TokenType type) : _location(pos)
{
  _type = type;
  _value = value;
}
