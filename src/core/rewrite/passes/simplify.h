#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "../../ast/binary.h"
#include "../../ast/expr.h"
#include "../../ast/function.h"
#include "../../ast/number.h"
#include "../../ast/symbol.h"

namespace latex_solver {

    class Simplifier {
        public:
        static ExprPtr simplify(const Expr &expr);

        private:
        static ExprPtr simplify_expr(const Expr &expr);

        static ExprPtr simplify_binary(const BinaryOp &bin);

        static ExprPtr simplify_function(const Function &func);
    };

} // namespace latex_solver

#endif // SIMPLIFY_H
