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

  void simplify(void);

  unsigned int get_degree(void) const { return (_degree); }

  friend std::ostream &
  operator<<(std::ostream &os, const Polynomial &poly);

  Polynomial &operator-=(const Polynomial &right);
  Polynomial operator-(const Polynomial &right);

  bool operator==(const Polynomial &right)
  {
    return (_degree == right._degree && _terms == right._terms);
  }

private:
  std::vector<Term> _terms;
  unsigned int _degree;

  std::vector<Term> _ast_to_terms(const ast::ExprPtr &expr);

  Term _handle_power(const ast::BinaryExpr *binary);
  std::vector<Term> _handle_multiply(
      const std::vector<Term> &left, const std::vector<Term> &right
  );
  std::vector<Term> _handle_binary(const ast::ExprPtr &expr);

  unsigned int _eval_constant_power(const ast::ExprPtr &expr);

  void _set_degree(void);
};
