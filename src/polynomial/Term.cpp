#include "polynomial/Term.hpp"
#include <algorithm>
#include <ostream>
#include <unordered_map>

Term::~Term(void) {}
Term::Term(void) : _coefficient(0), _max_degree(0) {}

Term::Term(float coefficient) : _coefficient(coefficient), _max_degree(0)
{
}

Term::Term(float coefficient, char variable, unsigned int degree)
    : _coefficient(coefficient), _degree({{variable, degree}}),
      _max_degree(degree)
{
}

Term &Term::operator*=(const Term &right)
{
  this->_coefficient *= right._coefficient;
  for (const auto &[var, deg] : right._degree)
  {
    _degree[var] += deg;
    _max_degree = std::max(_max_degree, _degree[var]);
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

unsigned int Term::get_max_degree(void) const { return (_max_degree); }

float Term::get_coefficient(void) const { return (_coefficient); }

const std::unordered_map<char, unsigned int> Term::get_degrees(void) const
{
  return (_degree);
}

std::ostream &operator<<(std::ostream &os, const Term &term)
{
  int prev_degree = 1;

  os << term.get_coefficient();
  for (const auto &[var, deg] : term.get_degrees())
  {
    if (prev_degree != 1)
      os << " * ";
    os << "(" << (int)var << ")";
    os << var;
    if (deg != 1)
      os << "^" << deg;
    prev_degree = deg;
  }
  return (os);
}
