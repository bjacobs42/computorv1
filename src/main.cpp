#include "../include/config.hpp"
#include <iostream>
#include <string>

int main(int ac, char **argv) {
  std::string input;

  if (ac <= 1) {
    std::cout << PROGRAM_NAME << ": " << USAGE << std::endl;
    return (0);
  }

  for (int i = 1; i < ac; ++i)
    input += argv[i];
  return (0);
}
