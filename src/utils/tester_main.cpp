#include "ast/ast.hpp"
#include "config.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int get_ast_height(const ast::ExprPtr &expression)
{
  if (!expression)
    return (0);
  if (expression->kind != ast::ExprKind::binary)
    return (1);
  ast::BinaryExpr *binary = (ast::BinaryExpr *)expression.get();
  // clang-format off
  return (1 + std::max(
    get_ast_height(binary->left),
    get_ast_height(binary->right)
  ));
  // clang-format on
}

void fill_grid(
    std::vector<std::string> &grid, const ast::ExprPtr &expression,
    int row, int col, int gap
)
{
  if (!expression)
    return;

  std::string value;
  switch ((int)expression->kind)
  {
  case (int)ast::ExprKind::binary:
    value = ((ast::BinaryExpr *)expression.get())->op.get_value();
    break;
  case (int)ast::ExprKind::number:
    value = std::to_string(((ast::NumberExpr *)expression.get())->value);
    break;
  case (int)ast::ExprKind::variable:
    value = ((ast::VariableExpr *)expression.get())->name;
    break;
  default:
    value = "unknown";
  }

  for (size_t i = 0; i < value.size(); ++i)
    grid[row][col + i] = value[i];

  if (expression->kind != ast::ExprKind::binary)
    return;

  int half_gap = gap / 2;
  ast::BinaryExpr *binary = (ast::BinaryExpr *)expression.get();

  if (binary->left)
  {
    grid[row + 1][col - half_gap] = '/';
    fill_grid(grid, binary->left, row + 2, col - gap, half_gap);
  }
  if (binary->right)
  {
    grid[row + 1][col + half_gap] = '\\';
    fill_grid(grid, binary->right, row + 2, col + gap, half_gap);
  }
}

void print_ast(const ast::ExprPtr &expression)
{
  int height = get_ast_height(expression);
  int width = (1 << height) * 2;
  int rows = height * 2 - 1;

  // Blank canvas
  std::vector<std::string> grid(rows, std::string(width, ' '));

  fill_grid(grid, expression, 0, width / 2, width / 4);

  for (const std::string &row : grid)
    std::cout << row << std::endl;
}

void print_tokens(const std::vector<Token> &tokens)
{
  static const std::string token_types[] = {
      "Number:",
      "Variable:",
      "Plus:",
      "Minus:",
      "Multiply:",
      "Divide:",
      "Exponent:"
  };

  std::stringstream outstream;
  for (const Token &token : tokens)
    outstream << token_types[(int)token.get_type()] << " "
              << token.get_value() << std::endl;
  std::cout << outstream.str();
}

void print_ast(const ast::Expr &expression)
{
  if (expression.kind != ast::ExprKind::binary)
  {
    return;
  }
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
  Parser parser(input);
  print_tokens(tokens);

  std::cout << "parsing..." << std::endl;
  ast::ExprPtr ast = parser.parse();

  std::cout << "printing..." << std::endl;
  print_ast(ast);
  return (0);
}
