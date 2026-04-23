#include "../../include/token.hpp"
#include <cctype>
#include <string>
#include <vector>

std::string get_value(const std::string &input, int (*func)(int)) {
  int end = 0;

  while (input[end] && func(end))
    ++end;
  return (input.substr(0, end));
}

std::vector<token> lexer(const std::string &input) {
  std::vector<token> tokens;
  int input_len = input.length();
  int i = 0;

  while (i < input_len) {
    char character = input[i];
    std::string value = "";
    token_type type;

    if (std::isspace(character)) {
      ++i;
      continue;
    }

    if (std::isdigit(character)) {
      value = get_value(&input[i], std::isdigit);
      type = token_type::NUMBER;
    } else if (std::isalpha(character)) {
      value += character;
      type = token_type::VARIABLE;
    } else if (std::isprint(character)) {
      value += character;
      type = token_type::OPERATOR;
    } else
      throw; // insert error

    tokens.push_back(token(value, i, type));
    i += value.length();
  }
  return (tokens);
}
