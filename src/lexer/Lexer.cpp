#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <cctype>

Lexer::Lexer(const std::string &input) : _input(input) {}
Lexer::~Lexer(void) {}

CharClass Lexer::_classify(char character)
{
  if (std::isdigit(character))
    return (CharClass::digit);
  if (std::isalpha(character))
    return (CharClass::alpha);
  if (std::isprint(character))
    return (CharClass::oper);
  return (CharClass::unknown);
}

Token Lexer::_handle_number(void)
{
  size_t start = _pos;
  bool has_dot = false;

  while (_input[_pos])
  {
    char character = _input[_pos];

    if (character == '.' && !has_dot)
      has_dot = true;
    else if (!std::isdigit(character))
      break;
    ++_pos;
  }
  return (
      Token(_input.substr(start, _pos - start), _pos, TokenType::NUMBER)
  );
}

Token Lexer::_handle_variable(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (Token(std::string(1, character), index, TokenType::VARIABLE));
}

Token Lexer::_handle_operator(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (
      Token(std::string(1, character), index, Token::get_type(character))
  );
}
Token Lexer::_handle_error(void) { throw std::exception(); }

inline bool is_implicit_multi(TokenType prev_type, TokenType current_type)
{
  return (
      (prev_type == TokenType::VARIABLE ||
       prev_type == TokenType::NUMBER) &&
      current_type == TokenType::VARIABLE
  );
}

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
    if (tokens.size() &&
        is_implicit_multi(tokens.back().get_type(), tok.get_type()))
    {
      tokens.push_back(Token(std::string(1, '*'), _pos, TokenType::STAR));
    }
    tokens.push_back(tok);
  }
  return (tokens);
}
