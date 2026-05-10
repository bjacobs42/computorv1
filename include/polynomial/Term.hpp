#pragma once

#include <unordered_map>
class Term
{
public:
  Term(void);
  Term(float coefficient, unsigned int degree = 0, char variable = 0);
  ~Term(void);

  Term &operator*=(const Term &right);
  Term &operator*=(float num);
  Term operator*(const Term &right) const;
  Term operator*(float num) const;

  float get_coefficient(void) const;
  const std::unordered_map<char, int> get_degrees(void) const;

private:
  float _coefficient;
  std::unordered_map<char, int> _degree;
};
