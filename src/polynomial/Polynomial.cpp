#include "polynomial/Polynomial.hpp"
#include "ast/ast.hpp"
#include "lexer/Token.hpp"
#include "polynomial/Term.hpp"
#include "utils/math.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

Polynomial::Polynomial(void) {}
Polynomial::~Polynomial(void) {}

Polynomial::Polynomial(const std::vector<Term> &terms) : _terms(terms)
{
  _set_max_degree();
}

Polynomial::Polynomial(const ast::ExprPtr &expr)
    : _terms(_ast_to_terms(expr))
{
  _set_max_degree();
}

unsigned int Polynomial::get_max_degree(void) const
{
  return (_max_degree);
}

void Polynomial::_set_max_degree(void)
{
  for (const Term &term : _terms)
    _max_degree = std::max(_max_degree, term.get_max_degree());
}

std::vector<Term> Polynomial::_handle_multiply(
    const std::vector<Term> &left, const std::vector<Term> &right
)
{
  std::vector<Term> result;

  result.reserve(left.size() * right.size());
  for (const Term &right_term : right)
  {
    for (const Term &left_term : left)
      result.push_back(left_term * right_term);
  }
  return (result);
}

unsigned int Polynomial::_eval_constant_power(const ast::ExprPtr &expr)
{
  if (expr->kind == ast::ExprKind::number)
  {
    const ast::NumberExpr *num_expr = (const ast::NumberExpr *)expr.get();
    return ((unsigned int)num_expr->value);
  }

  const ast::BinaryExpr *binary = (const ast::BinaryExpr *)expr.get();
  unsigned int base = _eval_constant_power(binary->left);
  unsigned int exponent = _eval_constant_power(binary->right);
  return (ft_math::power(base, exponent));
}

std::vector<Term> Polynomial::_handle_power(const ast::BinaryExpr *binary)
{
  unsigned int right = _eval_constant_power(binary->right);

  switch ((int)binary->left->kind)
  {
  case (int)ast::ExprKind::number:
  {
    const ast::NumberExpr *num_expr =
        (const ast::NumberExpr *)binary->left.get();
    return {{ft_math::power(num_expr->value, right)}};
  }
  case (int)ast::ExprKind::variable:
  {
    const ast::VariableExpr *var_expr =
        (const ast::VariableExpr *)binary->left.get();
    return {{1.0f, var_expr->name, right}};
  }
  default:
    throw std::runtime_error("Unsupported base in power expression");
  }
}

std::vector<Term> Polynomial::_handle_binary(const ast::ExprPtr &expr)
{
  const ast::BinaryExpr *binary = (const ast::BinaryExpr *)expr.get();
  TokenType operator_type = binary->op.get_type();

  if (operator_type == TokenType::CARET)
    return (_handle_power(binary));

  std::vector<Term> left = _ast_to_terms(binary->left);
  std::vector<Term> right = _ast_to_terms(binary->right);
  switch ((int)operator_type)
  {
  case (int)TokenType::PLUS:
    left.insert(
        left.end(),
        std::make_move_iterator(right.begin()),
        std::make_move_iterator(right.end())
    );
    break;
  case (int)TokenType::DASH:
    for (Term &term : right)
      term *= -1;
    left.insert(
        left.end(),
        std::make_move_iterator(right.begin()),
        std::make_move_iterator(right.end())
    );
    break;
  case (int)TokenType::STAR:
    left = _handle_multiply(left, right);
    break;
  }
  return (left);
}

std::vector<Term> Polynomial::_ast_to_terms(const ast::ExprPtr &expr)
{
  switch ((int)expr->kind)
  {
  case (int)ast::ExprKind::binary:
    return (_handle_binary(expr));
  case (int)ast::ExprKind::number:
    return {{((ast::NumberExpr *)expr.get())->value}};
  case (int)ast::ExprKind::variable:
    return {{1.0f, ((ast::VariableExpr *)expr.get())->name}};
  default:
    throw std::runtime_error("What the hell did you do?");
  }
}

std::ostream &operator<<(std::ostream &os, const Polynomial &poly)
{
  int term_count = poly._terms.size();

  for (int i = 0; i < term_count; ++i)
  {
    const Term &term = poly._terms[i];
    float coefficient = term.get_coefficient();

    if (i)
      os << (coefficient > 0.0f ? " + " : " - ");
    if (ft_math::abs(coefficient) != 1.0f)
      os << ft_math::abs(coefficient);

    int prev_degree = 1;
    for (const auto &[var, deg] : term.get_degrees())
    {
      if (prev_degree != 1)
        os << " * ";
      os << var;
      if (deg != 1)
        os << "^" << deg;
      prev_degree = deg;
    }
  }
  return (os);
}
