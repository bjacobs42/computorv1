#pragma once

#include <ostream>
#include <unordered_map>

class Term
{
public:
  Term(void);
  Term(float coefficient);
  Term(float coefficient, char variable, unsigned int degree = 1);
  ~Term(void);

  Term &operator*=(const Term &right);
  Term &operator*=(float num);
  Term operator*(const Term &right) const;
  Term operator*(float num) const;

  unsigned int get_max_degree(void) const;
  float get_coefficient(void) const;
  const std::unordered_map<char, unsigned int> get_degrees(void) const;

  friend std::ostream &operator<<(std::ostream &os, const Term &term);

private:
  float _coefficient;
  std::unordered_map<char, unsigned int> _degree;
  unsigned int _max_degree;
};
