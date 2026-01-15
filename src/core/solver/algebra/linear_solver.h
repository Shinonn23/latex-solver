#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include "../../../core/ast/equation.h"
#include "../../../core/ast/expr.h"
#include "../../../core/common/context.h"
#include <memory>
#include <set>
#include <string>

namespace latex_solver {
    class LinearSolver {
        static void   find_variables(const Expr            &expr,
                                     std::set<std::string> &vars,
                                     const Context         &ctx);

        static double collect_coefficient(const Expr        &expr,
                                          const std::string &var_name,
                                          const Context     &ctx);

        static double collect_constant(const Expr        &expr,
                                       const std::string &var_name,
                                       const Context     &ctx);

        public:
        static double solve(const Equation &eq, const Context &ctx = Context());
    };

} // namespace latex_solver

#endif
