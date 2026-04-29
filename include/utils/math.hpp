#pragma once

namespace ft_math
{
  template <typename Type = double> constexpr Type abs(const Type &num)
  {
    return (num < (Type)0 ? num * (Type)-1 : num);
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
  Type sqrt(const Type &base, const int &expression);
} // namespace ft_math
