#include "utils/utils.hpp"
#include "config.hpp"
#include "polynomial/Polynomial.hpp"
#include <array>
#include <iostream>
#include <stdexcept>

Polynomial create_polynomial(const std::string &input)
{
  Polynomial polynomial;

  try
  {
    std::array<Polynomial, 2> polynomials;

    size_t pos = input.find_first_of('=');
    if (pos == std::string::npos)
      polynomials[0] = Polynomial(input);
    else
    {
      polynomials[0] = Polynomial(input.substr(0, pos));
      polynomials[1] = Polynomial(input.substr(pos + 1));
    }
    polynomial = polynomials[0] - polynomials[1];
    if (polynomial.empty())
      throw std::runtime_error(USAGE);
  }
  catch (std::exception &e)
  {
    std::cout << PROGRAM_NAME << ": " << e.what() << std::endl;
  }
  return (polynomial);
}
