#include "parser/Parser.hpp"
#include "ast/ast.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

Parser::Parser(const std::string &input)
{
  Lexer lexer(input);
  _tokens = lexer.lex();
  _pos = 0;
}

Parser::~Parser() {}

inline Token Parser::_peek(void) const
{
  return (_tokens[_pos]);
}

Token Parser::_consume(void)
{
  Token token = _tokens[_pos];
  ++_pos;
  return (token);
}

ast::Expr Parser::parse(void)
{
  while (_has_tokens())
  {
  }

  return (ast::Expr());
}

inline TokenType Parser::_peek_type(void) const
{
  return (_peek().get_token_type());
}

inline bool Parser::_has_tokens(void) const
{
  return (
      _peek_type() != TokenType::END || _pos < _tokens.size()
  );
}
