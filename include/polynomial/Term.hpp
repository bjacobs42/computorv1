#pragma once

#include <map>
#include <ostream>

class Term
{
public:
  Term(void);
  Term(float coefficient, char variable, unsigned int degree = 1);
  Term(float coefficient, const std::map<char, unsigned int> &variables = {});
  ~Term(void);

  Term &operator*=(const Term &right);
  Term &operator*=(float num);
  Term operator*(const Term &right) const;
  Term operator*(float num) const;

  Term &operator-=(const Term &right);
  Term operator-(const Term &right);

  bool operator==(const Term &right) const;
  bool operator>(const Term &right) const
  {
    return (_max_exponent > right._max_exponent);
  }

  bool is_constant(void) const { return (_variables.empty()); }
  bool is_multivariable(void) const { return (_variables.size() > 1); }

  unsigned int get_max_exponent(void) const { return (_max_exponent); }
  float get_coefficient(void) const { return (_coefficient); }
  const std::map<char, unsigned int> &get_variables(void) const
  {
    return (_variables);
  }

  friend std::ostream &operator<<(std::ostream &os, const Term &term);

private:
  float _coefficient;
  std::map<char, unsigned int> _variables;
  unsigned int _max_exponent;
};
