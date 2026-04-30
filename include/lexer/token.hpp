#pragma once

#include <string>

enum class token_type
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

class token
{
public:
  token(const std::string &value, int pos);
  token(const std::string &value, int pos, token_type type);
  ~token(void);

  token_type get_token_type(void) const;
  static token_type get_token_type(char character)
  {
    switch (character)
    {
    case '+':
      return (token_type::PLUS);
    case '-':
      return (token_type::DASH);
    case '*':
      return (token_type::STAR);
    case '/':
      return (token_type::SLASH);
    case '^':
      return (token_type::CARET);
    case ';':
      return (token_type::END);
    default:
      throw std::exception();
    }
  }

private:
  int _location;
  token_type _type;
  std::string _value;

  token(void);
};
