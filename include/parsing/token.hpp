#pragma once

#include <string>

enum class token_type
{
  NUMBER,
  VARIABLE,
  OPERATOR,
  CARET,
  END
};

class token
{
public:
  token(const std::string &value, int pos);
  token(const std::string &value, int pos, token_type type);
  ~token(void);

  static token_type get_token_type(char character);
  token_type get_token_type(void) const;

private:
  int _location;
  token_type _type;
  std::string _value;

  token(void);
};
