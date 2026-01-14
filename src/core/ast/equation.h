#ifndef EQUATION_H
#define EQUATION_H

#include "expr.h"
#include <memory>
#include <string>

namespace latex_solver {

class Equation {
  private:
	ExprPtr left_;	// Left side of the equation
	ExprPtr right_; // Right side of the equation

  public:
	Equation(ExprPtr left, ExprPtr right)
		: left_(std::move(left)), right_(std::move(right)) {}

	const Expr &left() const { return *left_; }

	const Expr &right() const { return *right_; }

	ExprPtr &left_ptr() { return left_; }

	ExprPtr &right_ptr() { return right_; }

	std::string to_string() const {
		return left_->to_string() + " = " + right_->to_string();
	}

	std::unique_ptr<Equation> clone() const {
		return std::make_unique<Equation>(left_->clone(), right_->clone());
	}
};

using EquationPtr = std::unique_ptr<Equation>;

} // namespace latex_solver

#endif
