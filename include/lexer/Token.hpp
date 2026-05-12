#pragma once

#include <stdexcept>
#include <string>

enum class TokenType
{
  NUMBER,
  VARIABLE,
  PLUS,
  DASH,
  STAR,
  SLASH,
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

  const std::string &get_value(void) const;
  TokenType get_type(void) const;
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
    case '/':
      return (TokenType::SLASH);
    case '^':
      return (TokenType::CARET);
    case ';':
      return (TokenType::END);
    default:
      throw std::runtime_error("Token: unknown character");
    }
  }

private:
  size_t _location;
  TokenType _type;
  std::string _value;
};
