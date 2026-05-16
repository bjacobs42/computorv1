#include "parser/Parser.hpp"
#include "ast/ast.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <stdexcept>
#include <string>

Parser::Parser(const std::string &input)
{
  _init_lookup();
  _tokens = Lexer(input).lex();
  _pos = 0;
}

Parser::~Parser() {}

ast::ExprPtr Parser::parse(void)
{
  return (_parse_expr(BindingPower::default_bp));
}

const Token &Parser::_consume(void)
{
  const Token &token = _tokens[_pos];
  ++_pos;
  return (token);
}

TokenType Parser::_look_ahead(void) const
{
  if (_pos + 1 >= _tokens.size())
    return (TokenType::END);
  return (_tokens[_pos + 1].get_type());
}

ast::ExprPtr Parser::_parse_expr(BindingPower bp)
{
  NudFn nud_fn = _lookup[(int)_peek_type()].nud;
  if (!nud_fn)
    throw std::runtime_error("No nud function found");

  ast::ExprPtr left = (this->*nud_fn)();
  while (_has_tokens() && bp < _lookup[(int)_peek_type()].bp)
  {
    LedFn led_fn = _lookup[(int)_peek_type()].led;
    if (!led_fn)
      throw std::runtime_error("No led function found");

    left = (this->*led_fn)(std::move(left), _lookup[(int)_peek_type()].bp);
  }
  return (left);
}

ast::ExprPtr Parser::_parse_primary_expr(void)
{
  const Token &token = _consume();

  switch (token.get_type())
  {
  case TokenType::NUMBER:
  {
    float num = std::stof(token.get_value());
    return (ast::new_expr<ast::NumberExpr>(num));
  }
  case TokenType::PLUS:
    return (_parse_primary_expr());
  case TokenType::DASH:
  {
    ast::ExprPtr new_expr = _parse_primary_expr();
    if (new_expr->kind == ast::ExprKind::number)
      ((ast::NumberExpr *)new_expr.get())->value *= -1;
    else if (new_expr->kind == ast::ExprKind::variable)
      ((ast::VariableExpr *)new_expr.get())->sign *= -1;
    else
      throw std::runtime_error("Invalid minus target");
    return (new_expr);
  }
  case TokenType::VARIABLE:
    return (ast::new_expr<ast::VariableExpr>(token.get_value()[0]));
  default:
    throw std::runtime_error("Unknown TokenType in parse_primary");
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

ast::ExprPtr Parser::_parse_implicit_expr(ast::ExprPtr left, BindingPower bp)
{
  TokenType ahead_type = _look_ahead();
  if (_peek_type() == TokenType::NUMBER && ahead_type == TokenType::NUMBER)
  {
    throw std::runtime_error("Bad implicit expr");
  }

  Token operatorToken = Token(std::string(1, '*'), 0);
  ast::ExprPtr right = _parse_expr(bp);

  return (ast::new_expr<ast::BinaryExpr>(
      std::move(left), operatorToken, std::move(right)
  ));
}

void Parser::_register_nud(TokenType token_type, BindingPower bp, NudFn nud)
{
  Entry *entry = &_lookup[(int)token_type];

  entry->bp = bp;
  entry->nud = nud;
}

void Parser::_register_led(TokenType token_type, BindingPower bp, LedFn led)
{
  Entry *entry = &_lookup[(int)token_type];

  entry->bp = bp;
  entry->led = led;
}

// clang-format of
void Parser::_init_lookup(void)
{
  _register_nud(
      TokenType::NUMBER, BindingPower::number, &Parser::_parse_primary_expr
  );
  _register_nud(
      TokenType::VARIABLE, BindingPower::number, &Parser::_parse_primary_expr
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
      TokenType::PLUS, BindingPower::additive, &Parser::_parse_binary_expr
  );
  _register_led(
      TokenType::DASH, BindingPower::additive, &Parser::_parse_binary_expr
  );
  _register_led(
      TokenType::STAR, BindingPower::multiplicative, &Parser::_parse_binary_expr
  );
  _register_led(
      TokenType::SLASH,
      BindingPower::multiplicative,
      &Parser::_parse_binary_expr
  );
  _register_led(
      TokenType::CARET, BindingPower::exponential, &Parser::_parse_binary_expr
  );
}
