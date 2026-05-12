#pragma once

#include "Term.hpp"
#include "ast/ast.hpp"
#include <ostream>
#include <vector>

class Polynomial
{
public:
  Polynomial(void);
  Polynomial(const std::vector<Term> &terms);
  Polynomial(const ast::ExprPtr &expr);
  ~Polynomial(void);

  unsigned int get_max_degree(void) const;

  friend std::ostream &
  operator<<(std::ostream &os, const Polynomial &poly);

  Polynomial &operator-=(const Polynomial &right);
  Polynomial operator-(const Polynomial &right);

private:
  std::vector<Term> _terms;
  unsigned int _max_degree;

  std::vector<Term> _ast_to_terms(const ast::ExprPtr &expr);

  std::vector<Term> _handle_power(const ast::BinaryExpr *binary);
  std::vector<Term> _handle_multiply(
      const std::vector<Term> &left, const std::vector<Term> &right
  );
  std::vector<Term> _handle_binary(const ast::ExprPtr &expr);

  unsigned int _eval_constant_power(const ast::ExprPtr &expr);

  void _set_max_degree(void);
};
