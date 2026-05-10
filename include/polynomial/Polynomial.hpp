#pragma once

#include "Term.hpp"
#include "ast/ast.hpp"
#include <vector>

class Polynomial
{
public:
  Polynomial(void);
  Polynomial(const std::vector<Term> &terms);
  Polynomial(const ast::ExprPtr &expr);
  ~Polynomial(void);

private:
  std::vector<Term> _terms;

  std::vector<Term> _ast_to_terms(const ast::ExprPtr &expr);

  std::vector<Term> _handle_power(const ast::BinaryExpr *binary);
  std::vector<Term> _handle_multiply(
      const std::vector<Term> &left, const std::vector<Term> &right
  );
  std::vector<Term> _handle_binary(const ast::ExprPtr &expr);

  unsigned int _eval_constant_power(const ast::ExprPtr &expr);
};
