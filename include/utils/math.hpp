#pragma once

#define EPSILON 1e-9f

namespace ft_math
{
  template <typename Type = double> constexpr Type abs(const Type &num)
  {
    return (num < (Type)0 ? -num : num);
  }

  template <typename Type = double> constexpr Type square(const Type &base)
  {
    return (base * base);
  }

  template <typename Type = double>
  constexpr Type power(Type base, unsigned int exponent)
  {
    Type result = (Type)1;

    while (exponent)
    {
      if (exponent & 1)
        result *= base;
      base = ft_math::square(base);
      exponent >>= 1;
    }
    return (result);
  }

  template <typename Type = double>
  Type root(const Type &base, unsigned int exponent);

  template <typename Type = double> constexpr Type sqrt(const Type &base)
  {
    return (root(base, 2));
  }

  template <typename Type = float>
  constexpr bool flt_equal(const Type &a, const Type &b)
  {
    return (abs(a - b) < EPSILON);
  }
} // namespace ft_math
