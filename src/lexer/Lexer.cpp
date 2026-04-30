#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

Token Lexer::_handle_number(void)
{
  size_t end = _pos;

  while (_input[end] && std::isdigit(_input[end]))
    ++end;
  return (
      Token(_input.substr(_pos, end), _pos, TokenType::NUMBER)
  );
}

Token Lexer::_handle_variable(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (Token(
      std::string(1, character), index, TokenType::VARIABLE
  ));
}

Token Lexer::_handle_operator(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (Token(
      std::string(1, character),
      index,
      Token::get_token_type(character)
  ));
}
Token Lexer::_handle_error(void) { throw std::exception(); }

std::vector<Token> Lexer::lex()
{
  std::vector<Token> tokens;
  size_t input_len = _input.length();

  while (_pos < input_len)
  {
    char character = _input[_pos];

    if (std::isspace(character))
    {
      ++_pos;
      continue;
    }

    CharClass char_class = _classify(character);
    Token tok = (this->*_token_handlers[(int)char_class])();
    tokens.push_back(tok);
  }
  return (tokens);
}
