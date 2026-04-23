#include "../include/lexer.hpp"
#include <cctype>

lexer::lexer(const std::string &input) : _input(input) {}

std::string get_value(const std::string &input, int (*func)(int)) {
  int end = 0;

  while (input[end] && func(end))
    ++end;
  return (input.substr(0, end));
}

token_type lexer::_get_token_type(char character) {
  if (!this->_token_types.find(character))
    throw; // insert error here
  return (this->_token_types[character]);
}

token lexer::_newtoken(const std::string &value, int pos) {
  token token;

  token.col_index = pos;
  token.type = this->_get_token_type(value[0]);
  for (int i = 1; value[i]; ++i) {
  }
  return (token);
}

std::vector<token> lexer::lex(void) {
  int input_len = _input.length();
  int i;

  i = 0;
  while (i < input_len) {
    char character = _input[i];
    std::string value = "";

    if (std::isspace(character)) {
      ++i;
      continue;
    }

    if (std::isdigit(character))
      value = get_value(&_input[i], std::isdigit);
    else if (std::isalpha(character))
      value += character;
    else
      value = _token_type_to_value(_get_token_type(character));

    _tokens.push_back(_newtoken(value, i));
    i += value.length();
  }
  return (this->_tokens);
}

char lexer::_look_ahead(const std::string &input) {}
