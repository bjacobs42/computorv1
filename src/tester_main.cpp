#include "config.hpp"
#include "lexer/Lexer.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void print_tokens(const std::vector<Token> &tokens)
{
  static const std::string token_types[] = {
      "Number:", "Variable:", "Plus", "Minus", "Multiply", "Exponent"
  };

  std::stringstream outstream;
  for (const Token &token : tokens)
    outstream << token_types[(int)token.get_token_type()] << " "
              << token.get_value() << std::endl;
  std::cout << outstream.str();
}

int main(int ac, char **argv)
{
  std::string input;

  if (ac <= 1)
  {
    std::cout << PROGRAM_NAME << ": " << USAGE << std::endl;
    return (0);
  }

  for (int i = 1; i < ac; ++i)
    input += argv[i];

  std::vector<Token> tokens = Lexer(input).lex();
  return (0);
}
