#pragma once

#include <stdexcept>
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

  template <typename Type = float>
  constexpr bool flt_equal(const Type &a, const Type &b)
  {
    return (abs(a - b) < EPSILON);
  }

  template <typename Type = float>
  constexpr Type sqrt(const Type &base, float accruacy = 0.00001f)
  {
    if (base < 0.0f)
      throw std::runtime_error("sqrt does not handle complex numbers");

    if (flt_equal<Type>(base, 0.0f))
      return (0.0f);

    Type guess = base;
    Type prev = 0.0f;

    while (abs(prev - guess) > accruacy)
    {
      prev = guess;
      guess = 0.5f * (guess + base / guess);
    }

    return (guess);
  }
} // namespace ft_math
