#include "parser/Parser.hpp"
#include "ast/ast.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <string>

Parser::Parser(const std::string &input)
{
  Lexer lexer(input);

  _tokens = lexer.lex();
  _pos = 0;
}

Parser::~Parser() {}

ast::ExprPtr Parser::parse(void)
{
  return (_parse_expr(BindingPower::default_bp));
}

inline const Token &Parser::_peek(void) const { return (_tokens[_pos]); }

const Token &Parser::_consume(void)
{
  const Token &token = _tokens[_pos];
  ++_pos;
  return (token);
}

inline TokenType Parser::_peek_type(void) const
{
  return (_peek().get_type());
}

TokenType Parser::_look_ahead(void) const
{
  if (_pos + 1 >= _tokens.size())
    return (TokenType::END);
  return (_tokens[_pos + 1].get_type());
}

inline bool Parser::_has_tokens(void) const
{
  return (_peek_type() != TokenType::END || _pos < _tokens.size());
}

ast::ExprPtr Parser::_parse_expr(BindingPower bp)
{
  NudFn nud_fn = _lookup[(int)_peek_type()].nud;
  if (!nud_fn)
    throw; // implement error handler

  ast::ExprPtr left = (this->*nud_fn)();
  BindingPower current_bp = _lookup[(int)_peek_type()].bp;
  while (bp < current_bp && _has_tokens())
  {
    LedFn led_fn = _lookup[(int)_peek_type()].led;
    if (!led_fn)
      throw; // implement error handler

    left = (this->*led_fn)(std::move(left), current_bp);
    current_bp = _lookup[(int)_peek_type()].bp;
  }
  return (left);
}

ast::ExprPtr Parser::_parse_number_expr(void)
{
  const Token &token = _consume();

  switch (token.get_type())
  {
  case TokenType::NUMBER:
  {
    float num = std::stof(token.get_value());
    return (ast::new_expr<ast::NumberExpr>(num));
  }
  case TokenType::VARIABLE:
    return (ast::new_expr<ast::VariableExpr>(token.get_value()[0]));
  default:
    throw; // implement error handler!
  }
}

ast::ExprPtr Parser::_parse_binary_expr(ast::ExprPtr left, BindingPower bp)
{
  Token operatorToken = _consume();
  ast::ExprPtr right = _parse_expr(bp);

  return (ast::new_expr<ast::BinaryExpr>(
      std::move(left), operatorToken, std::move(right)
  ));
}

ast::ExprPtr
Parser::_parse_implicit_expr(ast::ExprPtr left, BindingPower bp)
{
  if (_peek_type() == TokenType::NUMBER &&
      _look_ahead() == TokenType::NUMBER)
  {
    throw; // Implement error
  }

  Token operatorToken = Token(std::string(1, '*'), 0);
  ast::ExprPtr right = _parse_expr(bp);

  return (ast::new_expr<ast::BinaryExpr>(
      std::move(left), operatorToken, std::move(right)
  ));
}

void Parser::_register_nud(
    TokenType token_type, BindingPower bp, NudFn nud
)
{
  Entry *entry = &_lookup[(int)token_type];

  entry->bp = bp;
  entry->nud = nud;
}

void Parser::_register_led(
    TokenType token_type, BindingPower bp, LedFn led
)
{
  Entry *entry = &_lookup[(int)token_type];

  entry->bp = bp;
  entry->led = led;
}

// clang-format off
void Parser::_init_lookup(void)
{
  _register_nud(
    TokenType::NUMBER,
    BindingPower::number,
    &Parser::_parse_number_expr
  );
  _register_nud(
    TokenType::VARIABLE,
    BindingPower::number,
    &Parser::_parse_number_expr
  );

  _register_led(
    TokenType::VARIABLE,
    BindingPower::multiplicative,
    &Parser::_parse_implicit_expr
  );
  _register_led(
    TokenType::NUMBER,
    BindingPower::multiplicative,
    &Parser::_parse_implicit_expr
  );
  _register_led(
    TokenType::DASH,
    BindingPower::additive,
    &Parser::_parse_binary_expr
  );
  _register_led(
    TokenType::DASH,
    BindingPower::additive,
    &Parser::_parse_binary_expr
  );
  _register_led(
    TokenType::STAR,
    BindingPower::multiplicative,
    &Parser::_parse_binary_expr
  );
  _register_led(
    TokenType::SLASH,
    BindingPower::multiplicative,
    &Parser::_parse_binary_expr
  );
  _register_led(
    TokenType::CARET,
    BindingPower::exponential,
    &Parser::_parse_binary_expr
  );
}
