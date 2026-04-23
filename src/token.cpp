#include "../include/token.hpp"
#include <string>

token::token(const std::string &value, int pos)
    : _location(pos), _type(token::get_token_type(value[pos])) {
  _value = value;
}
