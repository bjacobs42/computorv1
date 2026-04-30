#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include <cctype>
#include <exception>
#include <string>
#include <vector>

token lexer::_handle_number(void)
{
  size_t end = _pos;

  while (_input[end] && std::isdigit(_input[end]))
    ++end;
  return (token(
      _input.substr(_pos, end),
      _pos,
      token_type::NUMBER
  ));
}

token lexer::_handle_variable(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (token(
      std::string(1, character),
      index,
      token_type::VARIABLE
  ));
}

token lexer::_handle_operator(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (token(
      std::string(1, character),
      index,
      token::get_token_type(character)
  ));
}
token lexer::_handle_error(void)
{
  throw std::exception();
}

std::vector<token> lexer::lex()
{
  std::vector<token> tokens;
  size_t input_len = _input.length();

  while (_pos < input_len)
  {
    char character = _input[_pos];

    if (std::isspace(character))
    {
      ++_pos;
      continue;
    }

    char_class cls = _classify(character);
    token tok = (this->*_token_handlers[(int)cls])();
    tokens.push_back(tok);
  }
  return (tokens);
}
