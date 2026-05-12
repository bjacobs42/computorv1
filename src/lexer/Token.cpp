#include "lexer/Token.hpp"
#include <cctype>
#include <stdexcept>
#include <string>

Token::~Token(void) {}

Token::Token(const std::string &value, int pos) : _location(pos)
{
  if (value.empty())
    throw std::runtime_error("Token: Empty value");

  _type = Token::get_type(value[0]);
  _value = value;
}

Token::Token(const std::string &value, int pos, TokenType type)
    : _location(pos)
{
  _type = type;
  _value = value;
}

TokenType Token::get_type(void) const { return _type; };
const std::string &Token::get_value(void) const { return (_value); }
