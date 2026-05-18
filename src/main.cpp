#include "config.hpp"
#include "polynomial/Polynomial.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <string>

int main(int ac, char **argv)
{
  std::string input;
  Polynomial polynomial;
  Solution solution;

  if (ac <= 1)
  {
    std::cout << PROGRAM_NAME << ": " << USAGE << std::endl;
    return (0);
  }

  for (int i = 1; i < ac; ++i)
    input += argv[i];
  polynomial = create_polynomial(input);
  if (polynomial.empty())
    return (1);

  std::cout << "reduced form: " << polynomial << std::endl;
  polynomial.simplify();
  std::cout << "simplified form: " << polynomial << std::endl;
  std::cout << "Polynomial degree: " << polynomial.get_degree() << std::endl;

  solution = polynomial.solve();
  std::cout << solution.message << std::endl;
  for (float result : solution.results)
    std::cout << result << std::endl;
  return (0);
}
