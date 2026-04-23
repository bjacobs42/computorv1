#pragma once

#include <string>

enum class token_type { NUMBER, VARIABLE, OPERATOR, CARET, END };

class token {
public:
  token(const std::string &value, int pos);
  ~token(void);

  token_type get_token_type(char character);

private:
  token_type _type;
  std::string _value;
  int _location;

  token(void);
};
