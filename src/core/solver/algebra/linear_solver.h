#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include "../../../core/ast/binary.h"
#include "../../../core/ast/equation.h"
#include "../../../core/ast/expr.h"
#include "../../../core/ast/function.h"
#include "../../../core/ast/number.h"
#include "../../../core/ast/symbol.h"
#include "../../../core/common/context.h"
#include "../../../core/common/error.h"
#include "../../../core/common/evaluator.h"
#include <memory>
#include <set>
#include <string>

namespace latex_solver {

/**
 * Solver for linear equations of the form: ax + b = c
 * Where a, b, c are constants and x is the variable to solve for.
 *
 * Examples:
 * - "x + 5 = 10" -> x = 5
 * - "2 * x - 3 = 7" -> x = 5
 * - "10 = x + 5" -> x = 5
 */
class LinearSolver {
  private:
	/**
	 * Finds all unknown variables in an expression (excluding those in
	 * context).
	 *
	 * @param expr The expression to scan
	 * @param vars Set to collect variable names into
	 * @param ctx Context with known variable values
	 */
	static void find_variables(const Expr &expr, std::set<std::string> &vars,
							   const Context &ctx) {
		if (const auto *sym = dynamic_cast<const Symbol *>(&expr)) {
			if (!ctx.has(sym->name())) {
				vars.insert(sym->name());
			}
		} else if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
			find_variables(bin->left(), vars, ctx);
			find_variables(bin->right(), vars, ctx);
		} else if (const auto *func = dynamic_cast<const Function *>(&expr)) {
			find_variables(func->argument(), vars, ctx);
		}
	}

	/**
	 * Collects the coefficient of a variable in an expression.
	 * For example, in "2*x + 3", the coefficient of x is 2.
	 *
	 * @param expr The expression to analyze
	 * @param var_name The variable name to find coefficient for
	 * @param ctx Context for evaluating constants
	 * @return The coefficient of the variable
	 */
	static double collect_coefficient(const Expr &expr,
									  const std::string &var_name,
									  const Context &ctx) {
		// If expr is the variable itself, coefficient is 1
		if (const auto *sym = dynamic_cast<const Symbol *>(&expr)) {
			if (sym->name() == var_name) {
				return 1.0;
			}
			// It's a different variable, treat as constant 0
			return 0.0;
		}

		// If expr is a number, coefficient is 0
		if (dynamic_cast<const Number *>(&expr)) {
			return 0.0;
		}

		// If expr is a binary operation
		if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
			double left_coeff = collect_coefficient(bin->left(), var_name, ctx);
			double right_coeff =
				collect_coefficient(bin->right(), var_name, ctx);

			switch (bin->op()) {
			case BinaryOpType::ADD:
				return left_coeff + right_coeff;

			case BinaryOpType::SUB:
				return left_coeff - right_coeff;

			case BinaryOpType::MUL: {
				// Check if one side is a constant
				bool left_has_var = (left_coeff != 0.0);
				bool right_has_var = (right_coeff != 0.0);

				if (left_has_var && right_has_var) {
					throw SolverError(
						"Non-linear equation (variable appears multiple times "
						"in multiplication)");
				}

				if (left_has_var) {
					// left has variable, right is constant
					// e.g., x * 5 or (x+2) * 5
					Evaluator eval(ctx);
					double right_val = eval.evaluate(bin->right());
					return left_coeff * right_val;
				} else if (right_has_var) {
					// right has variable, left is constant
					// e.g., 5 * x or 5 * (x+2)
					Evaluator eval(ctx);
					double left_val = eval.evaluate(bin->left());
					return left_val * right_coeff;
				} else {
					// Neither has variable, coefficient is 0
					return 0.0;
				}
			}

			case BinaryOpType::DIV: {
				// Only support variable on left side: x / 2
				if (right_coeff != 0.0) {
					throw SolverError(
						"Cannot divide by expression containing variable");
				}

				if (left_coeff != 0.0) {
					Evaluator eval(ctx);
					double right_val = eval.evaluate(bin->right());
					if (right_val == 0.0) {
						throw SolverError("Division by zero");
					}
					return left_coeff / right_val;
				}

				return 0.0;
			}

			default:
				throw SolverError("Unknown binary operator");
			}
		}

		// If expr is a function call
		if (const auto *func = dynamic_cast<const Function *>(&expr)) {
			// Function calls with variables are non-linear
			double arg_coeff =
				collect_coefficient(func->argument(), var_name, ctx);
			if (arg_coeff != 0.0) {
				throw SolverError(
					"Non-linear equation (variable inside function)");
			}
			// Function with constant argument has coefficient 0
			return 0.0;
		}

		throw SolverError("Unsupported expression type");
	}

	/**
	 * Collects the constant term in an expression.
	 * For example, in "2*x + 3", the constant is 3.
	 *
	 * @param expr The expression to analyze
	 * @param var_name The variable name
	 * @param ctx Context for evaluating constants
	 * @return The constant term
	 */
	static double collect_constant(const Expr &expr,
								   const std::string &var_name,
								   const Context &ctx) {
		// If expr is the variable itself, constant is 0
		if (const auto *sym = dynamic_cast<const Symbol *>(&expr)) {
			if (sym->name() == var_name) {
				return 0.0;
			}
			// It's a different variable, try to evaluate
			if (ctx.has(sym->name())) {
				auto value = ctx.get(sym->name());
				if (value.has_value()) {
					return value.value();
				}
			}
			throw SolverError("Unknown variable: " + sym->name());
		}

		// If expr is a number, that's the constant
		if (const auto *num = dynamic_cast<const Number *>(&expr)) {
			return num->value();
		}

		// If expr is a binary operation
		if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
			double left_const = collect_constant(bin->left(), var_name, ctx);
			double right_const = collect_constant(bin->right(), var_name, ctx);

			switch (bin->op()) {
			case BinaryOpType::ADD:
				return left_const + right_const;

			case BinaryOpType::SUB:
				return left_const - right_const;

			case BinaryOpType::MUL: {
				// Check if both sides have constants
				double left_coeff =
					collect_coefficient(bin->left(), var_name, ctx);
				double right_coeff =
					collect_coefficient(bin->right(), var_name, ctx);

				if (left_coeff == 0.0 && right_coeff == 0.0) {
					// Both are constants, multiply them
					return left_const * right_const;
				}
				// If one side has variable, constant term is 0
				return 0.0;
			}

			case BinaryOpType::DIV: {
				double left_coeff =
					collect_coefficient(bin->left(), var_name, ctx);
				double right_coeff =
					collect_coefficient(bin->right(), var_name, ctx);

				if (left_coeff == 0.0 && right_coeff == 0.0) {
					// Both are constants, divide them
					if (right_const == 0.0) {
						throw SolverError("Division by zero");
					}
					return left_const / right_const;
				}
				// If one side has variable, constant term is 0
				return 0.0;
			}

			default:
				throw SolverError("Unknown binary operator");
			}
		}

		// If expr is a function call
		if (const auto *func = dynamic_cast<const Function *>(&expr)) {
			// Check if function argument contains the variable
			double arg_coeff =
				collect_coefficient(func->argument(), var_name, ctx);
			if (arg_coeff != 0.0) {
				// Variable inside function -> non-linear
				throw SolverError(
					"Non-linear equation (variable inside function)");
			}
			// Function with constant argument -> evaluate it
			Evaluator eval(ctx);
			return eval.evaluate(*func);
		}

		throw SolverError("Unsupported expression type");
	}

  public:
	/**
	 * Solves a linear equation by auto-detecting the variable.
	 * Equation must be in the form: a*x + b = c
	 *
	 * Algorithm:
	 * 1. Auto-detect the unknown variable in the equation
	 * 2. Convert equation to: left - right = 0
	 * 3. Collect coefficient 'a' and constant 'b' from (left - right)
	 * 4. Solve: a*x + b = 0 => x = -b/a
	 *
	 * @param eq The equation to solve
	 * @param ctx Context with known variable values
	 * @return The solution value
	 * @throws SolverError if equation is not linear or has no solution
	 */
	static double solve(const Equation &eq, const Context &ctx = Context()) {
		// Auto-detect the variable to solve for
		std::set<std::string> vars;
		find_variables(eq.left(), vars, ctx);
		find_variables(eq.right(), vars, ctx);

		if (vars.empty()) {
			throw SolverError("No unknown variables found in equation");
		}

		if (vars.size() > 1) {
			std::string var_list;
			for (const auto &v : vars) {
				var_list += v + " ";
			}
			throw SolverError("Multiple unknown variables found: " + var_list);
		}

		std::string var_name = *vars.begin();

		// Convert to form: (left - right) = 0
		// Collect coefficient and constant for the entire equation

		// Get coefficient from left side
		double left_coeff = collect_coefficient(eq.left(), var_name, ctx);
		double left_const = collect_constant(eq.left(), var_name, ctx);

		// Get coefficient from right side
		double right_coeff = collect_coefficient(eq.right(), var_name, ctx);
		double right_const = collect_constant(eq.right(), var_name, ctx);

		// Move everything to left side: (left - right) = 0
		// a*x + b = 0, where:
		double a = left_coeff - right_coeff; // coefficient of x
		double b = left_const - right_const; // constant term

		// Check if equation is actually linear
		if (a == 0.0) {
			if (b == 0.0) {
				throw SolverError("Equation is always true (infinite "
								  "solutions)");
			} else {
				throw SolverError("Equation has no solution");
			}
		}

		// Solve: a*x + b = 0 => x = -b/a
		return -b / a;
	}
};

} // namespace latex_solver

#endif // LINEAR_SOLVER_H
