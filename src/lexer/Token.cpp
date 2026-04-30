#include "lexer/Token.hpp"
#include <cctype>
#include <string>

Token::~Token(void) {}

Token::Token(const std::string &value, int pos)
    : _location(pos), _type(Token::get_token_type(value[0]))
{
  _value = value;
}

Token::Token(const std::string &value, int pos, TokenType type)
    : _location(pos)
{
  _type = type;
  _value = value;
}

TokenType Token::get_token_type(void) const { return _type; };
