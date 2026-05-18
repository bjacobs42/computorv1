#pragma once

#include "polynomial/Polynomial.hpp"
#include <string>

// takes a [polynomial] = [polynomial] as string input
// returns the subracted sum of the two polynomials or an empty polynomial if an
// error occurs
Polynomial create_polynomial(const std::string &input);
