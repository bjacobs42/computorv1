#pragma once

#include "token.hpp"
#include <string>
#include <vector>

std::vector<token> lexer(const std::string& input);
