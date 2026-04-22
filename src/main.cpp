#include "../include/config.hpp"
#include <iostream>

int main(int ac, char **argv) {
  if (ac <= 1)
    std::cout << PROGRAM_NAME << ": " << USAGE << std::endl;
}
