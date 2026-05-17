#pragma once

#include <stdexcept>
#include <string>
class TokenError : public std::runtime_error
{
public:
  TokenError(const std::string &msg, char character, int pos = -1)
      : std::runtime_error(_build_message(msg, character, pos)), _pos(pos),
        _char(character)

  {
  }

  int pos(void) const { return (_pos); }
  char character(void) const { return (_char); }

private:
  int _pos;
  char _char;

  std::string
  _build_message(const std::string &msg, char character, int pos) const
  {
    std::string position_msg = pos >= 0 ? ("at col" + std::to_string(pos)) : "";
    return ("Token: " + msg + " '" + character + "' " + position_msg);
  }
};
