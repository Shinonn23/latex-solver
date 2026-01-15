#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

#include "../../../core/ast/equation.h"
#include "../../../core/ast/expr.h"
#include "../../../core/common/context.h"
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace latex_solver {
    class QuadraticSolver {
        static void   find_variables(const Expr            &expr,
                                     std::set<std::string> &vars,
                                     const Context         &ctx);

        static double collect_quadratic_coefficient(const Expr        &expr,
                                                    const std::string &var_name,
                                                    const Context     &ctx);

        static double collect_linear_coefficient(const Expr        &expr,
                                                 const std::string &var_name,
                                                 const Context     &ctx);

        static double collect_constant(const Expr        &expr,
                                       const std::string &var_name,
                                       const Context     &ctx);

        public:
        static std::vector<double> solve(const Equation &eq,
                                         const Context  &ctx = Context());
    };

} // namespace latex_solver

#endif
