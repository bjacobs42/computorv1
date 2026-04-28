#pragma once

namespace ft_math
{

template <typename Type = double>
inline Type abs(const Type& num)
{
	return (num < (Type)0 ? num * (Type)-1 : num);
}

template <typename Type = double>
inline Type square(const Type& base)
{
	return (base * base);
}

template <typename Type = double>
Type power(const Type& base, const unsigned int& expression)
{
	Type sum = (Type)1;

	if (base == ft_math::abs<Type>(1))
		return (base);
	for (int i = 0; i < expression; ++i)
		sum *= base;
	return (sum);
}

template <typename Type = double>
Type sqrt(const Type& base, const int& expression);
} // namespace ft_math
