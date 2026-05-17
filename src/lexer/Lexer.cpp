#include "lexer/Lexer.hpp"
#include "lexer/LexerError.hpp"
#include "lexer/Token.hpp"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string &input) : _pos(0), _input(input) {}
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
  return (Token(_input.substr(start, _pos - start), _pos, TokenType::NUMBER));
}

Token Lexer::_handle_variable(void)
{
  size_t index = _pos;
  char character = std::tolower(_input[_pos++]);

  return (Token(std::string(1, character), index, TokenType::VARIABLE));
}

Token Lexer::_handle_operator(void)
{
  size_t index = _pos;
  char character = _input[_pos++];

  return (Token(std::string(1, character), index, Token::get_type(character)));
}

Token Lexer::_handle_error(void)
{
  throw LexerError("Non printable character", _input, _pos);
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
    tokens.push_back(tok);
  }
  if (!tokens.size())
    throw std::runtime_error("Lexer: Empty string");
  if (tokens.back().get_type() != TokenType::END)
    tokens.push_back(Token("", _pos, TokenType::END));
  return (tokens);
}
