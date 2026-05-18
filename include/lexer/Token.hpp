#pragma once

#include <string>

enum class TokenType
{
  NUMBER,
  VARIABLE,
  PLUS,
  DASH,
  STAR,
  CARET,
  END
};

class Token
{
public:
  Token(void);
  Token(const std::string &value, int pos);
  Token(const std::string &value, int pos, TokenType type);
  ~Token(void);

  size_t get_location(void) const { return (_location); }
  const std::string &get_value(void) const { return (_value); }
  TokenType get_type(void) const { return (_type); }
  static TokenType get_type(char character)
  {
    switch (character)
    {
    case '+':
      return (TokenType::PLUS);
    case '-':
      return (TokenType::DASH);
    case '*':
      return (TokenType::STAR);
    case '^':
      return (TokenType::CARET);
    case ';':
      return (TokenType::END);
    default:
      return ((TokenType)-1);
    }
  }

private:
  size_t _location;
  TokenType _type;
  std::string _value;
};
