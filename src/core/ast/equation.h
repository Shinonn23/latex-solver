#ifndef EQUATION_H
#define EQUATION_H

#include "expr.h"
#include <memory>
#include <string>

namespace latex_solver {

/**
 * Represents a mathematical equation with left and right sides.
 * Example: "x + 5 = 10" has left side "x + 5" and right side "10"
 */
class Equation {
  private:
	ExprPtr left_;	// Left side of the equation
	ExprPtr right_; // Right side of the equation

  public:
	/**
	 * Constructs an equation with the given left and right expressions.
	 *
	 * @param left The left-hand side expression
	 * @param right The right-hand side expression
	 */
	Equation(ExprPtr left, ExprPtr right)
		: left_(std::move(left)), right_(std::move(right)) {}

	/**
	 * Gets the left-hand side of the equation.
	 *
	 * @return Reference to the left expression
	 */
	const Expr &left() const { return *left_; }

	/**
	 * Gets the right-hand side of the equation.
	 *
	 * @return Reference to the right expression
	 */
	const Expr &right() const { return *right_; }

	/**
	 * Gets mutable access to the left-hand side.
	 *
	 * @return Pointer to the left expression
	 */
	ExprPtr &left_ptr() { return left_; }

	/**
	 * Gets mutable access to the right-hand side.
	 *
	 * @return Pointer to the right expression
	 */
	ExprPtr &right_ptr() { return right_; }

	/**
	 * String representation of the equation.
	 *
	 * @return String like "x + 5 = 10"
	 */
	std::string to_string() const {
		return left_->to_string() + " = " + right_->to_string();
	}

	/**
	 * Creates a deep copy of this equation.
	 *
	 * @return A unique pointer to the cloned equation
	 */
	std::unique_ptr<Equation> clone() const {
		return std::make_unique<Equation>(left_->clone(), right_->clone());
	}
};

using EquationPtr = std::unique_ptr<Equation>;

} // namespace latex_solver

#endif // EQUATION_H
