#include "polynomial/Term.hpp"
#include <unordered_map>

Term::~Term(void) {}
Term::Term(void) : _coefficient(0), _degree({}) {}
Term::Term(float coefficient, unsigned int degree, char variable)
    : _coefficient(coefficient), _degree({{variable, degree}})
{
}

Term &Term::operator*=(const Term &right)
{
  this->_coefficient *= right._coefficient;
  for (const auto &[var, deg] : right._degree)
    _degree[var] += deg;
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

float Term::get_coefficient(void) const { return (_coefficient); }

const std::unordered_map<char, int> Term::get_degrees(void) const
{
  return (_degree);
}
