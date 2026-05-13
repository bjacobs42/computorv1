#include "polynomial/Polynomial.hpp"
#include "ast/ast.hpp"
#include "lexer/Token.hpp"
#include "polynomial/PolynomialMessages.hpp"
#include "polynomial/Term.hpp"
#include "utils/math.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

Polynomial::Polynomial(void) {}
Polynomial::~Polynomial(void) {}

Polynomial::Polynomial(const std::vector<Term> &terms) : _terms(terms)
{
  _set_degree();
}

Polynomial::Polynomial(const ast::ExprPtr &expr)
    : _terms(_ast_to_terms(expr))
{
  _set_degree();
}

void Polynomial::simplify(void)
{
  std::map<std::map<char, unsigned int>, float> accumulator;

  for (const Term &term : _terms)
    accumulator[term.get_variables()] += term.get_coefficient();

  _terms.clear();
  for (const auto &[vars, coeff] : accumulator)
  {
    if (ft_math::flt_equal(coeff, 0.0f))
      continue;

    _terms.emplace_back(coeff, vars);
  }
  std::sort(_terms.begin(), _terms.end(), std::greater<Term>());
}

Solution Polynomial::_solve_monomial(void) const
{
  float a = _get_coefficient_by_degree(1);
  float b = _get_coefficient_by_degree(0);

  if (ft_math::flt_equal(a, 0.0f))
    return (Solution(NO_SOLUTIONS, {}, false));
  return (Solution(SOLUTION_IS, {-b / a}, true));
}

Solution Polynomial::_solve_quadratic(void) const
{
  Solution solution;

  return (solution);
}

Solution Polynomial::solve(void)
{
  Solution solution;

  if (_degree > 2)
  {
    solution.message = DEGREE_TOO_HIGH;
    return (solution);
  }
  if (is_multivariable())
  {
    solution.message = HAS_MULTIVARIABLE;
    return (solution);
  }

  switch (_degree)
  {
  case 0:
    solution.solved = _terms.empty();
    solution.message = solution.solved ? INFINITE_SOLUTIONS : NO_SOLUTIONS;
  case 1:
    solution = _solve_monomial();
  default:
    solution = _solve_quadratic();
  }

  return (solution);
}

float Polynomial::get_discriminant(void) const
{
  if (_degree != 2)
  {
    throw std::runtime_error(
        "Can't get the discriminant of a non-quadratic polynomial"
    );
  }

  if (is_multivariable())
  {
    throw std::runtime_error(
        "Can't get the discriminant of a multivariable polynomial"
    );
  }

  float a = _get_coefficient_by_degree(2);
  float b = _get_coefficient_by_degree(1);
  float c = _get_coefficient_by_degree(0);

  return (ft_math::square(b) - 4.0f * a * c);
}

bool Polynomial::is_multivariable(void) const
{
  for (const Term &term : _terms)
  {
    if (term.is_multivariable())
      return (true);
  }
  return (false);
}

Polynomial &Polynomial::operator-=(const Polynomial &right)
{
  int right_size = right._terms.size();
  int left_size = _terms.size();
  int i;

  for (i = 0; i < left_size && i < right_size; ++i)
    _terms[i] -= right._terms[i];
  while (i < right_size)
  {
    _terms.push_back(right._terms[i] * -1.0f);
    ++i;
  }
  return (*this);
}

Polynomial Polynomial::operator-(const Polynomial &right)
{
  Polynomial result = *this;
  result -= right;
  return (result);
}

void Polynomial::_set_degree(void)
{
  for (const Term &term : _terms)
    _degree = std::max(_degree, term.get_max_exponent());
}

std::vector<Term> Polynomial::_handle_multiply(
    const std::vector<Term> &left,
    const std::vector<Term> &right
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

std::vector<Term>::const_iterator
Polynomial::_get_term_by_degree(unsigned int exponent) const
{
  std::vector<Term>::const_iterator it = _terms.begin();
  for (; it != _terms.end(); ++it)
  {
    if (it->get_max_exponent() == exponent)
      return (it);
  }
  return (_terms.end());
}

float Polynomial::_get_coefficient_by_degree(unsigned int exponent) const
{
  std::vector<Term>::const_iterator it = _get_term_by_degree(exponent);
  if (it == _terms.end())
    return (0.0f);
  return (it->get_coefficient());
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

Term Polynomial::_handle_power(const ast::BinaryExpr *binary)
{
  unsigned int exponent = _eval_constant_power(binary->right);
  if (exponent == 0)
    return (Term(1.0f));

  switch ((int)binary->left->kind)
  {
  case (int)ast::ExprKind::number:
  {
    const ast::NumberExpr *num_expr =
        (const ast::NumberExpr *)binary->left.get();
    return (Term(ft_math::power(num_expr->value, exponent)));
  }
  case (int)ast::ExprKind::variable:
  {
    const ast::VariableExpr *var_expr =
        (const ast::VariableExpr *)binary->left.get();
    return (Term(1.0f, var_expr->name, exponent));
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
    return {_handle_power(binary)};

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

  if (term_count == 0)
  {
    os << '0';
    return (os);
  }

  for (int i = 0; i < term_count; ++i)
  {
    const Term &term = poly._terms[i];
    float coefficient = term.get_coefficient();

    if (i)
      os << (coefficient >= 0.0f ? " + " : " - ");
    if (term.is_constant() || ft_math::flt_equal(coefficient, 1.0f))
      os << ft_math::abs(coefficient);

    int prev_exponent = 1;
    for (const auto &[var, exp] : term.get_variables())
    {
      if (prev_exponent != 1)
        os << " * ";
      os << var;
      if (exp != 1)
        os << "^" << exp;
      prev_exponent = exp;
    }
  }
  return (os);
}
