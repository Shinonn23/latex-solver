#include "quadratic_solver.h"
#include "../../../core/ast/binary.h"
#include "../../../core/ast/equation.h"
#include "../../../core/ast/expr.h"
#include "../../../core/ast/function.h"
#include "../../../core/ast/number.h"
#include "../../../core/ast/symbol.h"
#include "../../../core/common/context.h"
#include "../../../core/common/error.h"
#include "../../../core/eval/evaluator.h"
#include <cmath>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace latex_solver {

    void QuadraticSolver::find_variables(const Expr            &expr,
                                         std::set<std::string> &vars,
                                         const Context         &ctx) {
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

    double QuadraticSolver::collect_quadratic_coefficient(
        const Expr &expr, const std::string &var_name, const Context &ctx) {
        if (dynamic_cast<const Symbol *>(&expr)) {
            return 0.0;
        }

        if (dynamic_cast<const Number *>(&expr)) {
            return 0.0;
        }

        if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
            double left_quad =
                collect_quadratic_coefficient(bin->left(), var_name, ctx);
            double right_quad =
                collect_quadratic_coefficient(bin->right(), var_name, ctx);

            switch (bin->op()) {
            case BinaryOpType::ADD:
                return left_quad + right_quad;

            case BinaryOpType::SUB:
                return left_quad - right_quad;

            case BinaryOpType::MUL: {
                double left_linear =
                    collect_linear_coefficient(bin->left(), var_name, ctx);
                double right_linear =
                    collect_linear_coefficient(bin->right(), var_name, ctx);

                // x * x = x^2
                if (left_linear != 0.0 && right_linear != 0.0) {
                    return left_linear * right_linear;
                }

                // c * x^2
                if (left_quad != 0.0) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
                    return left_quad * right_val;
                } else if (right_quad != 0.0) {
                    Evaluator eval(ctx);
                    double    left_val = eval.evaluate(bin->left());
                    return left_val * right_quad;
                }

                return 0.0;
            }

            case BinaryOpType::DIV: {
                if (right_quad != 0.0) {
                    throw SolverError("Cannot divide by expression containing "
                                      "variable squared");
                }

                if (left_quad != 0.0) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
                    if (right_val == 0.0) {
                        throw SolverError("Division by zero");
                    }
                    return left_quad / right_val;
                }

                return 0.0;
            }

            case BinaryOpType::POW: {
                double left_linear =
                    collect_linear_coefficient(bin->left(), var_name, ctx);

                // Check if base contains the variable
                if (left_linear != 0.0 || left_quad != 0.0) {
                    // Variable is in the base, check if exponent is 2
                    Evaluator eval(ctx);
                    double    exponent = eval.evaluate(bin->right());

                    if (exponent == 2.0 && left_linear != 0.0 &&
                        left_quad == 0.0) {
                        // x^2 case
                        return 1.0;
                    } else if (exponent != 2.0 && exponent != 1.0) {
                        throw SolverError(
                            "Non-quadratic power (exponent must be 2 or 1)");
                    }
                }

                // Variable is not in the base, result is constant
                return 0.0;
            }

            default:
                throw SolverError("Unknown binary operator");
            }
        }

        if (const auto *func = dynamic_cast<const Function *>(&expr)) {
            double arg_quad =
                collect_quadratic_coefficient(func->argument(), var_name, ctx);
            if (arg_quad != 0.0) {
                throw SolverError(
                    "Variable squared inside function (non-quadratic)");
            }
            return 0.0;
        }

        throw SolverError("Unsupported expression type");
    }

    double QuadraticSolver::collect_linear_coefficient(
        const Expr &expr, const std::string &var_name, const Context &ctx) {
        if (const auto *sym = dynamic_cast<const Symbol *>(&expr)) {
            if (sym->name() == var_name) {
                return 1.0;
            }
            return 0.0;
        }

        if (dynamic_cast<const Number *>(&expr)) {
            return 0.0;
        }

        if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
            double left_linear =
                collect_linear_coefficient(bin->left(), var_name, ctx);
            double right_linear =
                collect_linear_coefficient(bin->right(), var_name, ctx);

            switch (bin->op()) {
            case BinaryOpType::ADD:
                return left_linear + right_linear;

            case BinaryOpType::SUB:
                return left_linear - right_linear;

            case BinaryOpType::MUL: {
                double left_quad =
                    collect_quadratic_coefficient(bin->left(), var_name, ctx);
                double right_quad =
                    collect_quadratic_coefficient(bin->right(), var_name, ctx);

                bool left_has_var  = (left_linear != 0.0 || left_quad != 0.0);
                bool right_has_var = (right_linear != 0.0 || right_quad != 0.0);

                // x * x case handled in quadratic coefficient
                if (left_linear != 0.0 && right_linear != 0.0) {
                    return 0.0; // This becomes x^2
                }

                // c * x
                if (left_has_var) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
                    return left_linear * right_val;
                } else if (right_has_var) {
                    Evaluator eval(ctx);
                    double    left_val = eval.evaluate(bin->left());
                    return left_val * right_linear;
                }

                return 0.0;
            }

            case BinaryOpType::DIV: {
                if (right_linear != 0.0) {
                    throw SolverError(
                        "Cannot divide by expression containing variable");
                }

                if (left_linear != 0.0) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
                    if (right_val == 0.0) {
                        throw SolverError("Division by zero");
                    }
                    return left_linear / right_val;
                }

                return 0.0;
            }

            case BinaryOpType::POW: {
                double left_quad =
                    collect_quadratic_coefficient(bin->left(), var_name, ctx);

                // Check if base contains the variable
                if (left_linear != 0.0 || left_quad != 0.0) {
                    // Variable is in the base, check if exponent is 1
                    Evaluator eval(ctx);
                    double    exponent = eval.evaluate(bin->right());

                    if (exponent == 1.0 && left_linear != 0.0) {
                        // x^1 case
                        return 1.0;
                    } else if (exponent == 2.0) {
                        // x^2 case - handled by quadratic coefficient
                        return 0.0;
                    } else if (exponent != 1.0 && exponent != 2.0 &&
                               exponent != 0.0) {
                        throw SolverError(
                            "Non-quadratic power (exponent must be 2 or 1)");
                    }
                }

                // Variable is not in the base or exponent is 0
                return 0.0;
            }

            default:
                throw SolverError("Unknown binary operator");
            }
        }

        if (const auto *func = dynamic_cast<const Function *>(&expr)) {
            double arg_linear =
                collect_linear_coefficient(func->argument(), var_name, ctx);
            if (arg_linear != 0.0) {
                throw SolverError("Variable inside function (non-quadratic)");
            }
            return 0.0;
        }

        throw SolverError("Unsupported expression type");
    }

    double QuadraticSolver::collect_constant(const Expr        &expr,
                                             const std::string &var_name,
                                             const Context     &ctx) {
        if (const auto *sym = dynamic_cast<const Symbol *>(&expr)) {
            if (sym->name() == var_name) {
                return 0.0;
            }
            if (ctx.has(sym->name())) {
                auto value = ctx.get(sym->name());
                if (value.has_value()) {
                    return value.value();
                }
            }
            throw SolverError("Unknown variable: " + sym->name());
        }

        if (const auto *num = dynamic_cast<const Number *>(&expr)) {
            return num->value();
        }

        if (const auto *bin = dynamic_cast<const BinaryOp *>(&expr)) {
            double left_const  = collect_constant(bin->left(), var_name, ctx);
            double right_const = collect_constant(bin->right(), var_name, ctx);

            switch (bin->op()) {
            case BinaryOpType::ADD:
                return left_const + right_const;

            case BinaryOpType::SUB:
                return left_const - right_const;

            case BinaryOpType::MUL: {
                double left_linear =
                    collect_linear_coefficient(bin->left(), var_name, ctx);
                double right_linear =
                    collect_linear_coefficient(bin->right(), var_name, ctx);
                double left_quad =
                    collect_quadratic_coefficient(bin->left(), var_name, ctx);
                double right_quad =
                    collect_quadratic_coefficient(bin->right(), var_name, ctx);

                bool left_has_var  = (left_linear != 0.0 || left_quad != 0.0);
                bool right_has_var = (right_linear != 0.0 || right_quad != 0.0);

                if (!left_has_var && !right_has_var) {
                    return left_const * right_const;
                }
                return 0.0;
            }

            case BinaryOpType::DIV: {
                double left_linear =
                    collect_linear_coefficient(bin->left(), var_name, ctx);
                double right_linear =
                    collect_linear_coefficient(bin->right(), var_name, ctx);
                double left_quad =
                    collect_quadratic_coefficient(bin->left(), var_name, ctx);
                double right_quad =
                    collect_quadratic_coefficient(bin->right(), var_name, ctx);

                bool left_has_var  = (left_linear != 0.0 || left_quad != 0.0);
                bool right_has_var = (right_linear != 0.0 || right_quad != 0.0);

                if (!left_has_var && !right_has_var) {
                    if (right_const == 0.0) {
                        throw SolverError("Division by zero");
                    }
                    return left_const / right_const;
                }
                return 0.0;
            }

            case BinaryOpType::POW: {
                double left_linear =
                    collect_linear_coefficient(bin->left(), var_name, ctx);
                double left_quad =
                    collect_quadratic_coefficient(bin->left(), var_name, ctx);

                bool left_has_var = (left_linear != 0.0 || left_quad != 0.0);

                // If base has variable, the result is not constant
                if (left_has_var) {
                    return 0.0;
                }

                // Both base and exponent are constants
                Evaluator eval(ctx);
                return eval.evaluate(*bin);
            }

            default:
                throw SolverError("Unknown binary operator");
            }
        }

        if (const auto *func = dynamic_cast<const Function *>(&expr)) {
            double arg_linear =
                collect_linear_coefficient(func->argument(), var_name, ctx);
            double arg_quad =
                collect_quadratic_coefficient(func->argument(), var_name, ctx);
            if (arg_linear != 0.0 || arg_quad != 0.0) {
                throw SolverError("Variable inside function (non-quadratic)");
            }
            Evaluator eval(ctx);
            return eval.evaluate(*func);
        }

        throw SolverError("Unsupported expression type");
    }

    std::vector<double> QuadraticSolver::solve(const Equation &eq,
                                               const Context  &ctx) {
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

        double      left_quad =
            collect_quadratic_coefficient(eq.left(), var_name, ctx);
        double left_linear =
            collect_linear_coefficient(eq.left(), var_name, ctx);
        double left_const = collect_constant(eq.left(), var_name, ctx);

        double right_quad =
            collect_quadratic_coefficient(eq.right(), var_name, ctx);
        double right_linear =
            collect_linear_coefficient(eq.right(), var_name, ctx);
        double right_const = collect_constant(eq.right(), var_name, ctx);

        // Collect to standard form: ax^2 + bx + c = 0
        double a           = left_quad - right_quad;
        double b           = left_linear - right_linear;
        double c           = left_const - right_const;

        // Check if it's actually quadratic
        if (a == 0.0) {
            throw SolverError("Not a quadratic equation (use linear solver)");
        }

        // Calculate discriminant
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0.0) {
            throw SolverError("No real solutions (discriminant is negative)");
        }

        std::vector<double> solutions;

        if (discriminant == 0.0) {
            // One solution
            double x = -b / (2 * a);
            solutions.push_back(x);
        } else {
            // Two solutions
            double sqrt_disc = std::sqrt(discriminant);
            double x1        = (-b + sqrt_disc) / (2 * a);
            double x2        = (-b - sqrt_disc) / (2 * a);
            solutions.push_back(x1);
            solutions.push_back(x2);
        }

        return solutions;
    }

} // namespace latex_solver
