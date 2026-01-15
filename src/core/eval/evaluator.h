#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../ast/expr.h"
#include "../common/context.h"

namespace latex_solver {

    class Evaluator : public ExprVisitor {
        private:
        const Context &context_;
        double         result_;

        public:
        explicit Evaluator(const Context &ctx);

        double evaluate(const Expr &expr);

        void   visit(const Number &node) override;
        void   visit(const Symbol &node) override;
        void   visit(const BinaryOp &node) override;
        void   visit(const Function &node) override;
    };

} // namespace latex_solver

#endif
