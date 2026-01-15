#include "linear_solver.h"
#include "../../../core/ast/binary.h"
#include "../../../core/ast/equation.h"
#include "../../../core/ast/expr.h"
#include "../../../core/ast/function.h"
#include "../../../core/ast/number.h"
#include "../../../core/ast/symbol.h"
#include "../../../core/common/context.h"
#include "../../../core/common/error.h"
#include "../../../core/eval/evaluator.h"
#include <memory>
#include <set>
#include <string>

namespace latex_solver {

    void LinearSolver::find_variables(const Expr            &expr,
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

    double LinearSolver::collect_coefficient(const Expr        &expr,
                                             const std::string &var_name,
                                             const Context     &ctx) {
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
            double left_coeff = collect_coefficient(bin->left(), var_name, ctx);
            double right_coeff =
                collect_coefficient(bin->right(), var_name, ctx);

            switch (bin->op()) {
            case BinaryOpType::ADD:
                return left_coeff + right_coeff;

            case BinaryOpType::SUB:
                return left_coeff - right_coeff;

            case BinaryOpType::MUL: {
                bool left_has_var  = (left_coeff != 0.0);
                bool right_has_var = (right_coeff != 0.0);

                if (left_has_var && right_has_var) {
                    throw SolverError("Non-linear equation (variable "
                                      "appears multiple times "
                                      "in multiplication)");
                }

                if (left_has_var) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
                    return left_coeff * right_val;
                } else if (right_has_var) {
                    Evaluator eval(ctx);
                    double    left_val = eval.evaluate(bin->left());
                    return left_val * right_coeff;
                } else {
                    return 0.0;
                }
            }

            case BinaryOpType::DIV: {
                if (right_coeff != 0.0) {
                    throw SolverError(
                        "Cannot divide by expression containing variable");
                }

                if (left_coeff != 0.0) {
                    Evaluator eval(ctx);
                    double    right_val = eval.evaluate(bin->right());
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

        if (const auto *func = dynamic_cast<const Function *>(&expr)) {
            double arg_coeff =
                collect_coefficient(func->argument(), var_name, ctx);
            if (arg_coeff != 0.0) {
                throw SolverError(
                    "Non-linear equation (variable inside function)");
            }
            return 0.0;
        }

        throw SolverError("Unsupported expression type");
    }

    double LinearSolver::collect_constant(const Expr        &expr,
                                          const std::string &var_name,
                                          const Context     &ctx) {
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
                double left_coeff =
                    collect_coefficient(bin->left(), var_name, ctx);
                double right_coeff =
                    collect_coefficient(bin->right(), var_name, ctx);

                if (left_coeff == 0.0 && right_coeff == 0.0) {
                    return left_const * right_const;
                }
                return 0.0;
            }

            case BinaryOpType::DIV: {
                double left_coeff =
                    collect_coefficient(bin->left(), var_name, ctx);
                double right_coeff =
                    collect_coefficient(bin->right(), var_name, ctx);

                if (left_coeff == 0.0 && right_coeff == 0.0) {
                    if (right_const == 0.0) {
                        throw SolverError("Division by zero");
                    }
                    return left_const / right_const;
                }
                return 0.0;
            }

            default:
                throw SolverError("Unknown binary operator");
            }
        }

        if (const auto *func = dynamic_cast<const Function *>(&expr)) {
            double arg_coeff =
                collect_coefficient(func->argument(), var_name, ctx);
            if (arg_coeff != 0.0) {
                throw SolverError(
                    "Non-linear equation (variable inside function)");
            }
            Evaluator eval(ctx);
            return eval.evaluate(*func);
        }

        throw SolverError("Unsupported expression type");
    }

    double LinearSolver::solve(const Equation &eq, const Context &ctx) {
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

        std::string var_name   = *vars.begin();

        double      left_coeff = collect_coefficient(eq.left(), var_name, ctx);
        double      left_const = collect_constant(eq.left(), var_name, ctx);

        double right_coeff     = collect_coefficient(eq.right(), var_name, ctx);
        double right_const     = collect_constant(eq.right(), var_name, ctx);

        double a               = left_coeff - right_coeff; // coefficient of x
        double b               = left_const - right_const; // constant term

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

} // namespace latex_solver