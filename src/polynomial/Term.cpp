#include "polynomial/Term.hpp"
#include "utils/math.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

Term::~Term(void) {}
Term::Term(void) : _coefficient(0), _max_exponent(0) {}

Term::Term(float coefficient, char variable, unsigned int exponent)
    : _coefficient(coefficient), _variables({{variable, exponent}}),
      _max_exponent(exponent)
{
}

Term::Term(float coefficient, const std::map<char, unsigned int> &variables)
    : _coefficient(coefficient), _variables(variables), _max_exponent(0)
{
  for (const auto &[var, exp] : variables)
    _max_exponent = std::max(_max_exponent, exp);
}

Term &Term::operator*=(const Term &right)
{
  this->_coefficient *= right._coefficient;
  for (const auto &[var, exp] : right._variables)
  {
    _variables[var] += exp;
    _max_exponent = std::max(_max_exponent, _variables[var]);
  }
  return (*this);
}

Term &Term::operator*=(float num)
{
  this->_coefficient *= num;
  return (*this);
}

Term Term::operator*(const Term &right) const
{
  Term result = *this;
  result *= right;
  return (result);
}

Term Term::operator*(float num) const
{
  Term result = *this;
  result *= num;
  return (result);
}

Term &Term::operator-=(const Term &right)
{
  _coefficient -= right.get_coefficient();
  return (*this);
}

Term Term::operator-(const Term &right)
{
  Term result = *this;
  result -= right;
  return (right);
}

bool Term::operator==(const Term &right) const
{
  return (
      ft_math::flt_equal(_coefficient, right._coefficient) &&
      _variables == right._variables
  );
}

std::ostream &operator<<(std::ostream &os, const Term &term)
{
  int prev_exponent = 1;

  os << term.get_coefficient();
  for (const auto &[var, deg] : term.get_variables())
  {
    if (prev_exponent != 1)
      os << " * ";
    os << var;
    if (deg != 1)
      os << "^" << deg;
    prev_exponent = deg;
  }
  return (os);
}
