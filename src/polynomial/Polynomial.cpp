#include "polynomial/Polynomial.hpp"
#include "ast/ast.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "polynomial/PolynomialMessages.hpp"
#include "polynomial/Term.hpp"
#include "utils/math.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Polynomial::Polynomial(void) {}
Polynomial::~Polynomial(void) {}

Polynomial::Polynomial(const std::string &input)
{
  Parser parser(input);

  ast::ExprPtr expressions = parser.parse();
  _terms = _ast_to_terms(expressions);
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
  _set_degree();
}

Solution Polynomial::_solve_monomial(void) const
{
  float a = _get_coefficient_by_degree(1);
  float b = _get_coefficient_by_degree(0);

  if (ft_math::flt_equal(a, 0.0f))
    return {UNDEFINED};
  float x = -b / a;
  x = ft_math::flt_equal(x, 0.0f) ? 0.0f : x;
  return {SOLUTION_IS, {x}};
}

Solution Polynomial::_solve_quadratic(void) const
{
  float a = _get_coefficient_by_degree(2);
  float b = _get_coefficient_by_degree(1);
  float c = _get_coefficient_by_degree(0);

  if (ft_math::flt_equal(a, 0.0f))
    return {UNDEFINED};

  float denominator = 2 * a;
  float discriminant = get_discriminant(a, b, c);
  if (ft_math::flt_equal(discriminant, 0.0f))
    return {ONE_SOLUTION, {-b / denominator}};

  if (discriminant > 0.0f)
  {
    float root = ft_math::sqrt(discriminant);
    float positive = (-b + root) / denominator;
    float negative = (-b - root) / denominator;

    return {TWO_SOLUTIONS, {positive, negative}};
  }

  std::stringstream solution_msg;
  float imaginary = ft_math::sqrt(-discriminant) / ft_math::abs(denominator);
  float real = -b / denominator;

  solution_msg << COMPLEX_SOLUTION << std::endl;
  solution_msg << real << " + " << imaginary << "i" << std::endl;
  solution_msg << real << " - " << imaginary << "i" << std::endl;
  return {solution_msg.str()};
}

Solution Polynomial::solve(void)
{
  if (is_multivariable())
    return {HAS_MULTIVARIABLE};

  switch (_degree)
  {
  case 0:
    return {_terms.empty() ? INFINITE_SOLUTIONS : NO_SOLUTIONS};
  case 1:
    return (_solve_monomial());
  case 2:
    return (_solve_quadratic());
  default:
    return {DEGREE_TOO_HIGH};
  }
}

float Polynomial::get_discriminant(void) const
{
  if (_degree != 2)
  {
    throw std::runtime_error(
        "Polynomial: Can't get the discriminant of a non-quadratic polynomial"
    );
  }

  if (is_multivariable())
  {
    throw std::runtime_error(
        "Polynomial: Can't get the discriminant of a multivariable polynomial"
    );
  }

  float a = _get_coefficient_by_degree(2);
  float b = _get_coefficient_by_degree(1);
  float c = _get_coefficient_by_degree(0);

  return (ft_math::square(b) - 4.0f * a * c);
}

bool Polynomial::is_multivariable(void) const
{
  char first_var = 0;
  for (const Term &term : _terms)
  {
    if (term.is_multivariable())
      return (true);

    const std::map<char, unsigned int> &variables = term.get_variables();
    if (variables.empty())
      continue;

    char var_name = variables.begin()->first;
    if (!first_var)
      first_var = var_name;
    else if (first_var != var_name)
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
  _degree = 0;

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
  if (expr->kind == ast::ExprKind::variable)
  {
    throw std::runtime_error(
        "Polynomial: We do not accept variable power like x^x >:C; "
    );
  }

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

  switch (binary->left->kind)
  {
  case ast::ExprKind::number:
  {
    const ast::NumberExpr *num_expr =
        (const ast::NumberExpr *)binary->left.get();
    float value = ft_math::power(num_expr->value, exponent);

    if (std::isinf(value))
    {
      throw std::runtime_error(
          std::string(
              "Polynomial: Constant expression overflows: " +
              std::to_string(num_expr->value) + "^" + std::to_string(exponent)
          )
      );
    }
    return {value};
  }
  case ast::ExprKind::variable:
  {
    const ast::VariableExpr *var_expr =
        (const ast::VariableExpr *)binary->left.get();
    return (Term(1.0f, var_expr->name, exponent));
  }
  default:
    throw std::runtime_error(
        "Polynomial: Unsupported ast expression in power expression"
    );
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
  switch (operator_type)
  {
  case TokenType::PLUS:
    left.insert(
        left.end(),
        std::make_move_iterator(right.begin()),
        std::make_move_iterator(right.end())
    );
    break;
  case TokenType::DASH:
    for (Term &term : right)
      term *= -1;
    left.insert(
        left.end(),
        std::make_move_iterator(right.begin()),
        std::make_move_iterator(right.end())
    );
    break;
  case TokenType::STAR:
    left = _handle_multiply(left, right);
    break;
  default:
    throw std::runtime_error("Polynomial: unimplemented token type");
  }
  return (left);
}

std::vector<Term> Polynomial::_ast_to_terms(const ast::ExprPtr &expr)
{
  if (!expr)
    return {};

  switch (expr->kind)
  {
  case ast::ExprKind::binary:
    return (_handle_binary(expr));
  case ast::ExprKind::number:
    return {{((ast::NumberExpr *)expr.get())->value}};
  case ast::ExprKind::variable:
  {
    ast::VariableExpr *var = (ast::VariableExpr *)expr.get();
    return {{(float)var->sign, var->name}};
  }
  default:
    throw std::runtime_error("Polynomial: Unknown expresion kind");
  }
}

std::ostream &operator<<(std::ostream &os, const Polynomial &poly)
{
  int term_count = poly._terms.size();

  if (term_count == 0)
    os << '0';

  for (int i = 0; i < term_count; ++i)
  {
    const Term &term = poly._terms[i];
    float coefficient = term.get_coefficient();

    if (i)
      os << (coefficient >= 0.0f ? " + " : " - ");
    if (term.is_constant() ||
        !ft_math::flt_equal(ft_math::abs(coefficient), 1.0f))
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
  os << " = 0";
  return (os);
}
