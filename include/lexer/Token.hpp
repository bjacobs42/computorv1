#pragma once

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
  Token(const std::string &value, int pos);
  Token(const std::string &value, int pos, TokenType type);
  ~Token(void);

  const std::string &get_value(void) const;
  TokenType get_token_type(void) const;
  static TokenType get_token_type(char character)
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
      throw std::exception();
    }
  }

private:
  size_t _location;
  TokenType _type;
  std::string _value;
};
