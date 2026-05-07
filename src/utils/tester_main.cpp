#include "ast/ast.hpp"
#include "config.hpp"
#include "lexer/Lexer.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int get_ast_height(const ast::Expr *expression)
{
  if (!expression)
    return (0);
  if (expression->kind != ast::ExprKind::binary)
    return (1);
  // clang-format off
  return (1 + std::max(
    get_ast_height(((ast::BinaryExpr*)expression)->left),
    get_ast_height(((ast::BinaryExpr*)expression)->right)
  ));
  // clang-format on
}

void fill_grid(
    std::vector<std::string> &grid, const ast::Expr *expression, int row,
    int col, int gap
)
{
  if (!expression)
    return;

  std::string value;
  switch ((int)expression->kind)
  {
  case (int)ast::ExprKind::binary:
    value = ((ast::BinaryExpr *)expression)->op->get_value();
    break;
  case (int)ast::ExprKind::number:
    value = std::to_string(((ast::NumberExpr *)expression)->value);
    break;
  case (int)ast::ExprKind::variable:
    value = ((ast::VariableExpr *)expression)->name;
  default:
    value = "unknown";
  }

  for (size_t i = 0; i < value.size(); ++i)
    grid[row][col + i] = value[i];

  if (expression->kind != ast::ExprKind::binary)
    return;

  int half_gap = gap / 2;
  const ast::BinaryExpr *left =
      (ast::BinaryExpr *)((ast::BinaryExpr *)expression)->left;
  const ast::BinaryExpr *right =
      (ast::BinaryExpr *)((ast::BinaryExpr *)expression)->right;

  if (left)
  {
    grid[row + 1][col - half_gap] = '/';
    fill_grid(grid, left, row + 2, col - gap, half_gap);
  }
  if (right)
  {
    grid[row + 1][col + half_gap] = '\\';
    fill_grid(grid, right, row + 2, col + gap, half_gap);
  }
}

void print_ast(const ast::Expr *expression)
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
  print_tokens(tokens);
  return (0);
}
