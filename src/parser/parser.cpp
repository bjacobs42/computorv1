#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"
#include "utils.hpp"

parser::parser(const std::string &input) : _tokens(lexer(input)), _pos(0) {}
parser::~parser() {}

inline token parser::_peek(void) const { return (_tokens[_pos]); }

token parser::_consume(void)
{
  token tok = _tokens[_pos];
  ++_pos;
  return (tok);
}

ast::expr parser::parse(void)
{
  while (_has_tokens())
  {
  }

  return (ast::expr());
}

inline token_type parser::_peek_type(void) const
{
  return (_peek().get_token_type());
}

inline bool parser::_has_tokens(void) const
{
  return (_peek_type() != token_type::END || _pos < _tokens.size());
}
