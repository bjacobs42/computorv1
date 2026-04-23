#include "../include/token.hpp"
#include <string>

token::~token(void) {}

token::token(const std::string &value, int pos)
    : _location(pos), _type(token::get_token_type(value[0])) {
  _value = value;
}

token::token(const std::string &value, int pos, token_type type)
    : _location(pos) {
  _type = type;
  _value = value;
}

static token_type get_token_type(char character) {}
